= VLNontheFly

Paper：#link("https://arxiv.org/abs/2609.20191v1")[VLN on the Fly: An Onboard Vision-Language Navigation Stack for Aerial Robots]

Github：#link("https://github.com/peDrontonio/VLNontheFly")[VLNontheFly]

== Abstract

论文要解决的核心问题是：在算力有限的UAV上完全机载地运行视觉语言导航（VLN），这意味着语言定位、规划控制必须共享有限的算力。

端到端的空中策略将这些阶段融合进同一个网络，放弃了模块化系统栈所保留的可观测性与安全检查能力，导致飞行中某一阶段出现的错误很难被单独定位。论文提出的VLN on the Fly机载系统栈，将定位、规划和控制保持为互相独立、可检查的阶段。一个量化的VLM将指令定位到图像中的一个粗粒度网格单元，深度信息将其提升为三维目标点，一个快速的B-spline规划器给出可行轨迹，再由一个预训练的RAPTOR策略将其跟踪并转换为电机指令，且可适用于不同的四旋翼。

在受控室内空间中针对三种日常目标物进行的 15 次机载飞行里，该系统栈在 15 次试验中有 13 次到达目标，平均目标误差为 5.72 cm，平均 GPU 利用率为 39.3%。在另外 6 次杂乱环境试验中，该系统栈在机载感知门控下跟踪了无碰撞轨迹。

== Introduction

VLA模型已被用于满足这一需求：训练单个端到端策略，将原始观测和指令直接映射为控制指令。然而，语言模型取得成功所依赖的组合泛化能力，在物理自主系统中被证明更难实现。对于空中机器人，每条新指令不仅要在语义上下文中被定位，还必须在载具动力学和安全约束下对应到可行的运动。当定位、空间推理和控制被编码在单一端到端策略内部时，有限的可观测性使故障更难定位，安全约束也更难通过碰撞检测、可行性测试和恢复行为等标准规划机制来检查或执行。

近期文献表明，将这些模型解耦能够为导航系统栈带来理想的特性：各独立模块保持可观测，可复用的路径规划安全防护依然可用，并且修改系统栈中的某一组件无需重新训练整个策略。对于解耦方法而言，如何集成分层系统栈，尤其是与飞控内部模块的集成，仍是一个合理且在文献中探讨不足的问题。

该论文具体关注三个问题：
+ 安全性：VLM 的输出可能出现幻觉，目标可能飞出场地，轨迹可能撞上障碍物，因此每一级都需要检查。
+ 分层复杂性：传统飞控采用 位置 → 速度 → 姿态 → 角速率 的 PID 级联，每一级都要针对机型的质量、惯量、电机常数和推力极限调参。
+ 机载算力：VLM、建图、规划和控制都使用机载算力。

该论文有如下贡献：
- 一个机载、解耦的空中VLN系统栈，结合了用于open-vocabulary grounding的量化VLM、用于三维B-spline规划的 EGO-Planner，以及用于底层控制的预训练RAPTOR，无需端到端训练，也无需机外算力。
- 一个轻量的safety supervisor，使用finite-state machine拒绝超出作业边界的投影目标点，并依据机载占据地图上的规划器可行性对轨迹执行进行门控。
- 在包含三种目标物的open-vocabulary object-goal任务上，对完整机载系统栈进行的真实飞行可行性评估，以及得益于模块化设计的逐阶段故障归因。

== 任务流程

#figure(
	image("../assets/VLNontheFly/overview.png", width: 90%),
	caption: [Data Stream]
)

整个系统运行在 ROS 2 Humble 上，三类计算单元分工如下：Jetson Orin NX 运行 VLM、安全门控和 EGO-Planner；Pixhawk 6C 飞控运行 EKF2 和 RAPTOR；OptiTrack 动捕负责提供位姿。各数据流的反馈频率相差很大：VLM 约 0.5 Hz，规划器的 `pos_cmd` 约 100 Hz，RAPTOR 在飞控上的控制频率约 100 Hz。

+ Localization

	OptiTrack 动作捕捉系统输出MoCap Pose，交由 PX4 飞控的 EKF2 与 IMU 融合，输出的Pose给Goal Projection使用，输出的Odometry供 EGO-Planner 使用。

	Pose和Odometry都是来自同一个EKF2融合状态：

		- Pose是某一时刻的6DoF的（x，y，z）和四元数，用途是坐标变换。VLM在RGB图中指出目标，经过Depth反投影之后，得到相机坐标系下的3D点。之后要用无人机的Pose把这个点变换到世界坐标系，作为导航目标。

		- Odometry的内容是Pose，Twist（线速度，角速度），协方差和坐标系信息。用于轨迹规划，EGO-Planner每次重规划B-Spline都需要当前的位置、速度信息作为轨迹初始状态，才能生成动力学上连续可追踪的轨迹。如果仅有Pose的信息，EGO-Planner会把初速度当成0来规划，会出现轨迹跳变。

+ VLM选格子

	系统以0.5Hz的固定频率取一帧RGB，与系统提示词和用户提示词一起送进VLM。系统提示词把图像划成 3x3 的网格，让模型从9个区域名中选一个包含最符合用户提示词指示的物体的格子，输出包含这些信息的Json文件。

+ 目标投影

	之后系统解析 JSON，按 RGB 帧的时间戳找到深度相机输出的对齐的深度帧，取所选格子内有效深度的中位数，从格子中心像素反投影到相机坐标系，再经 TF 转到机体坐标系和地图坐标系。

+ 安全门控

	只有同一格子连续 3 次被选中、置信度都不低于 0.5、数据足够新，并且当前没有激活中的目标时，系统才会向EGO-Planner发布目标。EGO-Planner 的 FSM 收到目标后，再做一次地理围栏检查（场地立方体加禁飞圆柱）。

+ 轨迹规划

	EGO-Planner 根据深度图和位姿建立局部栅格地图，用B-Spline优化出一条光滑、可行、避障的轨迹，再由 `traj_server` 以约 100 Hz 采样成 `PositionCommand`（位置、速度、偏航）。经可行性检查通过后交给RAPTOR处理。

+ 底层控制

	飞控上的`rl_tools_commander` 工作在 PX4 EXTERNAL 模式下，RAPTOR 策略据此直接输出电机转速。External Mode是ROS2写的，动态注册到PX4的飞行模式。

+ 闭环

	无人机移动后，相机看到的新画面会进入下一轮 VLM 查询。两次查询之间，规划器和 RAPTOR 照常按自身频率工作。到达目标后，规划器回报 `reached`，门控节点释放当前目标，开始接受新的提议。

== 基于 VLM 的目标定位

论文使用量化为 INT4 的 Qwen-3.5-2B，将显存占用降低到足以进行机载推理的程度。每次 VLM 查询时，模型接收RGB图像帧和指令，然后从3×3图像网格中返回九个命名区域之一并附带置信度分数，或报告目标不可见。该网格仅在文本提示中定义。返回的区域在后处理阶段映射到对应的网格单元。将输出限制为九个区域名称，免去了像素坐标，使结果更易于校验。

#table(
	columns: (1fr, 1fr, 1fr),
	align: center,
	[TOP-LEFT], [TOP-CENTER], [TOP-RIGHT],
	[MIDDLE-LEFT], [CENTER], [MIDDLE-RIGHT],
	[BOTTOM-LEFT], [BOTTOM-CENTER], [BOTTOM-RIGHT],
)

对于返回的网格单元，相机坐标系下的对应点由反投影得到：

$
  P_"camera" = d_"cell"
  mat(
    (u_"cell" - c_x) \/ f_x;
    (v_"cell" - c_y) \/ f_y;
    1;
  ),
$

其中：
- $(u_"cell", v_"cell")$：VLM 选中的3×3网格单元的中心像素坐标。
- $d_"cell"$：该单元内有效深度值的中位数，也就是这个点沿相机光轴 Z 方向的距离。
- $f_x, f_y$：以像素为单位的焦距。
- $(c_x, c_y)$：主点，即光轴穿过图像平面的位置，通常接近图像中心。
- 向量 $((u - c_x)/f_x, (v - c_y)/f_y, 1)^T$ 是归一化坐标，也就是这个像素对应的射线方向，它在 Z = 1 平面上的交点。把它乘以深度 $d$，就把射线拉伸到真实距离处，得到 $(X, Y, Z)^T = (d(u-c_x)/f_x, d(v-c_y)/f_y, d)^T$。

这样就得到了相机坐标系下的目标坐标。

#block(
	width: 100%,
	inset: 8pt,
	stroke: 0.5pt + gray,
	radius: 4pt,
)[
	为什么用格子中位数而不是单个像素？ 论文的消融实验显示，从单个像素读深度，只有 15% 的帧有效，因为单个像素常落在低纹理、反光或超量程的表面上，还可能越过物体边界读到背景，与格子中位数的偏差最大可达 0.8 m。
]

=== 后处理

+ 光学坐标系 → 机体坐标系。用 RGB 帧拍摄时刻的 TF（`camera_to_body`）完成变换。机体坐标系 `base_link` 中 $x$ 向前、$y$ 向左，直接按轴对应：

	$
		x_"body" = z_"optical", quad y_"body" = -x_"optical"
	$

	这里只取水平分量，$y_"optical"$（竖直方向）被舍弃，因为高度会单独锁定。

+ 停靠距离（standoff）。反投影得到的是目标物表面上的点，直接飞过去会撞上，所以沿水平方向回退 $s = 0.8 "m"$。记 $D = sqrt(Delta x^2 + Delta y^2)$：

	$
		(Delta x', Delta y') = cases(
			(0, 0) & "if" D <= s,
			(D - s) / D dot (Delta x, Delta y) & "otherwise"
		)
	$

+ 单步距离限幅。一次发布的目标水平距离不超过 $L = 1.5 "m"$，超出时等比缩放：

	$
		(Delta x'', Delta y'') = min(1, L / D') dot (Delta x', Delta y')
	$

	因此远处的目标要分多步接近，每走一步，VLM 都能在新视角下重新确认。

+ 机体坐标系 → 地图坐标系，并锁定高度。再用拍摄时刻的 `body_to_map` 变换得到地图坐标 $(x_g, y_g)$。高度取 $z_g = z_"current"$，目标高度被约束在当前飞行高度附近的窄带内，将运动先局限在平面上，简化运动控制，给VLM充分的时间缓冲。

#block(
	width: 100%,
	inset: 8pt,
	stroke: 0.5pt + gray,
	radius: 4pt,
)[
	时间戳对齐：VLM推理需要约0.8s，这段时间里无人机还在动。如果用最新位姿去变换0.8s前那帧图像里的点，目标就会偏。所以实现中，门控节点保存最近300帧深度，按该时间戳找同步误差不超过0.1s的深度帧，TF查询也用同一时间戳。源帧超过0.8s则直接拒绝。
]

== 安全监督器

安全监督器负责监督VLM到规划器的交接，论文把它描述为有限状态机。

=== 作业边界

以起飞点 $(x_0, y_0, z_0)$ 为中心定义一个有界的立方体作业空间，候选目标 $(x_g, y_g, z_g)$ 必须满足：

$
	|x_g - x_0| <= b_x, quad |y_g - y_0| <= b_y, quad |z_g - z_0| <= b_z
$

其中 $b_x, b_y, b_z$ 是各轴限值。


=== 一致性共识与目标生命周期

每次收到 VLM 结果，门控节点先产生一个 `CellDecision`。以下任一情况都会被拒绝：目标不可见、置信度低于 0.5、区域名非法、格子内无有效深度、没有和 RGB 帧同步的深度。

监督器要求只有连续3次都选中同一个格子，才算稳定：

```python
def observe(self, decision):
    if not decision.accepted or decision.region is None:
        self.region, self.count = None, 0  # 任何一次无效都清零
        return False
    if decision.region == self.region:
        self.count = min(self.required, self.count + 1)
    else:
        self.region, self.count = decision.region, 1 # 换格子从1重新数
    return self.count >= self.required
```

目标被接受后，它将保持激活状态，直到 EGO-Planner 报告完成、拒绝或失败。邻近的提议和快速更新会被抑制，以防止在 VLM 查询之间产生不稳定的指令。在0.5Hz的查询频率下，连续3次共识至少需要约6s，论文用响应速度换稳定性。

目标生命周期可以概括成一个简单的状态机：
#align(
	center,
	table(
		columns: (auto, auto, auto),
		align: (center, center, center),
		table.header([当前状态], [下一状态], [触发条件]),
		[IDLE], [ACTIVE], [共识 $>= 3$ 且发布前检查全部通过，发布目标],
		[ACTIVE], [IDLE], [规划器回报 `reached` / `rejected` / `failed`],
		[ACTIVE], [IDLE], [所选格子深度连续 2 次失效],
	)
)

== 三维规划：EGO-Planner

EGO-Planner是这套系统的中间层，上游的VLM输出Goal Point，下游的RAPTOR负责把setpoints转换成电机转速，中间的路径规划，避障，平滑运动，速度控制由EGO-Planner负责。

- 输入：

	- Goal Point：门控节点发布的一个地图坐标系三维点。
	- Odometry：位置和速度，作为轨迹的起始状态。
	- Depth：用来构建局部Occupancy Grid。

- 输出：

	- traj_server每10ms采样一次B-Spline，将包含位置、速度、偏航信息的PositionCommand发给RAPTOR。

之所以使用EGO-Planner，是因为Fast-Planner 这类传统梯度规划器要维护全局 ESDF，开销太大。EGO-Planner 只在轨迹真正穿过障碍物的那一段做局部计算，单次规划通常只要几毫秒。可以减轻Jetson Orin NX的算力负担。

=== 一次规划的内部流程

+ 建图

	用Depth和Odometry做射线投射，得到Occupancy Grid。项目参数如下：

		- 分辨率 0.1 m
		- 障碍物膨胀 0.20 m
		- 射线最长 4.0 m
		- 局部更新范围 5.5 × 5.5 × 4.5 m

	膨胀相当于把无人机的体积加到了障碍物上。

+ 生成初始轨迹

	用当前位置、速度和目标点生成一条多项式轨迹，这一步不考虑障碍物。起点需要用到当前位置和速度，所以飞行中重规划也不会让轨迹跳变。

+ 取局部目标

	沿初始轨迹取水平面7.5m处的点作为局部目标。

+ Rebound优化

	+	把初始轨迹离散成均匀B-Spline的Setpoints ${Q_i}$。
	+ 检查每个Setpoint是否落进障碍物。对连续碰撞的那一段，用 A\* 搜一条绕障路径，并为每个碰撞Setpoint生成 ${p_(i j), v_(i j)}$：
			- $p_(i j)$ 是障碍物表面上的锚点；
			- $v_(i j)$ 是指向障碍物外的方向；
			- Setpoint到障碍物的距离为 $d_(i j) = (Q_i - p_(i j)) dot v_(i j)$。
			- 这就是无ESDF的含义：只在需要的地方构造局部距离。
	+ 最小化 $J = lambda_s J_s + lambda_c J_c + lambda_d J_d + ...$。碰撞项会把Setpoint往 $v_(i j)$ 方向"推"出去，这就是rebound的由来。优化中如果又发现新的碰撞，就再生成新的 ${p, v}$，继续优化。
	+ 如果速度或加速度超限，就按比例拉长时间并重新拟合。

+ 为什么用 B-Spline

	- 凸包性：只要约束Setpoint，整条曲线就满足速度、加速度和避障约束，不必逐点检查。
	- 局部支撑性：改一个Setpoint只影响相邻几段，所以优化和重规划都是局部的。

== 底层控制：RAPTOR

Paper：#link("https://arxiv.org/abs/2509.11481")[RAPTOR: A Foundation Policy for Quadrotor Control]

GitHub：#link("https://github.com/rl-tools/raptor")[raptor]

=== 从级联PID到Foundation Policy

传统PX4控制链每一级都要按机体的质量、惯量、电机常数和推力极限设置，换一台机架，或者加了负载，往往要重新调参：

$
	"位置" ->_"P" "速度" ->_"PID" "姿态" ->_"P" "角速率" ->_"PID" "电机混控"
$

RAPTOR是一个四旋翼Foundation Policy：它在大量随机化的四旋翼动力学参数上用强化学习训练，从而能零样本迁移到未见过的机架。网络只有2,084个参数、3层循环层，循环层让它可以从历史状态中隐式辨识当前机体的动力学。它直接运行在飞控上，输入为相对setpoint的状态（位置误差、姿态、速度、角速度等），输出为4个电机指令，替代了上面整条级联。在该论文提出的系统栈里，能够减少控制层的复杂性。

== 实验结果

平台为开源四旋翼 MIRA，搭载 Jetson Orin NX、RealSense D435i 和 Pixhawk 6C，由 OptiTrack 提供里程计。每次飞行从同一起飞位姿出发，每种目标物飞 5 次。

#align(
	center,
	table(
	columns: (auto, auto, auto, auto, auto),
	align: center,
	table.header([目标物], [成功率], [目标误差 (cm)], [跟踪误差 (cm)], [GPU占用率 (%)]),
	[垃圾桶], [1.00], [0.00], [34.96], [42.2],
	[椅子], [0.80], [10.07], [26.13], [37.3],
	[灭火器], [0.80], [7.08], [37.64], [38.4],
	[总体], [0.87], [5.72], [32.91], [39.3],
)
)


- 失败归因：两次失败中，无人机都在朝正确的物体接近，但飞近后目标离开相机视野，VLM 无法再修正目标，最终停在 20 cm 成功半径之外。解耦设计使这类失败能明确定位到定位阶段缺少空间记忆。
- 延迟：$N = 1865$ 次查询，首 token 时间中位数 0.60 s，总耗时中位数 0.79 s，第 95 百分位 0.88 s，足以支撑 0.5 Hz 的查询频率。
- 开放词汇：对同一批帧只替换提示词中的目标名，椅子 86% 落在左上格，垃圾桶 90% 落在中心格，说明模型能区分在场的不同物体。不在场的灭火器有 66% 被正确报告为不可见；剩下的情况里模型倾向于选中心格，属于幻觉，所以对场景外的目标，应更信任不可见的回答。
- 杂乱环境（$n = 6$）：3 次无碰撞到达目标，2 次安全不执行，1 次避障成功但跟踪偏差使最终距离 0.21 m，超出成功半径。四次实际执行的飞行平均跟踪误差为 17.6 cm。

== 局限性

实验证明了论文提出的VLN任务的可行性，但是模块化和安全性的收益是以集成上的权衡为代价的：

	- 目标在两次查询之间固定，因此一次定位错误可能会持续到下一次查询。
	- 没有空间记忆，只能看当前帧，所以视野外的目标必须先被纳入视野内才能被定位，限制了需要上下文或多步指令的任务。
	- 双目深度相机的有效量程有限，存在无效区域。

此外，论文做了两项简化假设，以专注于研究系统栈的运行情况：

	- z-clamp锁定了飞行高度，EGO-Planner的三维能力未得到发挥。
	- 所有试验都在OptiTrack场地内、从已知初始位姿开始，未评估任意初始化或重定位的情形。

#pagebreak()
