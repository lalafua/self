#import "../lib/common.typ": algorithm

= ASENA

Paper：#link("https://arxiv.org/abs/2609.39207v2")[ASENA: Self-evolving Agents for Embodied Navigation]

Homepage：#link("https://research.nvidia.com/labs/gear/asena/")[ASENA]

== Abstract

论文提出的ASENA是一个具身智能体系统，让通用的Coding Agent（Codex、Claude Code）使用固定权重的大模型（Claude Sonnet 5、GPT-6 Astra）通过编程(Code as Policy)来控制人形机器人完成导航、操作和交互任务。通过一个统一的Robot Harness，Agent可以读取传感器数据、编写并执行程序、检查执行记录、修复失败，并把笔记和可执行技能保存下来复用。

论文同时提出了ASENA-VLN，一个4B参数的单目导航策略，作为这套可编程系统中的可选工具。它预测机体坐标系下的轨迹，既能跟随长路线指令，也能执行“穿过门”、“前进1米”这类短程行为。

- 单独作为策略，ASENA-VLN在R2R和RxR上分别取得68.7%和70.2%的成功率（SOTA）。
- 集成到Coding Agent中，ASENA-VLN让较弱的Agent在两个Agentic Benchmark上的成功率都提高11个百分点，并减少执行时间。
- 工作区自进化：在同一组100个任务上重复10轮，R2R成功率从72%提升到98%，RxR从65%提升到89%。
- 在具身问答（EQA）上以更少的交互步数取得SOTA准确率。
- 在Unitree G1上无需预建地图，演示了搜索、视觉检查、空间推理、手势生成，以及抓取、搬运、递交物体的移动操作。

== Introduction

导航是一个大任务的一部分，下一步做什么取决于机器人看到了什么，机器人需要决定去哪里搜索、识别物品、记住用户位置并汇报结果。学习型导航策略只提供了移动能力，但一个具体的任务还需要决定检查什么、如何操作物体、和用户交流什么，这些流程依赖新的观测，常常超出预定义Skill的范围。

因此，论文的思路是：让Coding Agent在任务进行中通过编程访问感知、计算、仿真和执行接口，现场构建和修改这些流程；同时提供与结果绑定的反馈，以及一个持久化的工作区来保留有用的Skill。

论文的三点贡献：
- Robot Harness，让Coding Agent编程控制人形机器人的导航、操作和交互，并保留有用的Skills与经验。
- SOTA的单目导航策略ASENA-VLN，同时支持路线跟随和可复用的原子技能。
- 实验证明，在模型权重不变的情况下，工作区进化（Workspace Evolution）能有效提升任务表现。

与前面几篇论文对比：AGC-VLN和VLNontheFly都是由VLM输出一个格式固定的目标（像素点或格子），再由确定性的几何模块执行；ASENA把输出形式换成了程序，对应综述中的程序化策略规划（Policy Code Planning），并加入了从执行记录中自我修复和积累技能的机制，也就是反馈辅助规划（Plan with Feedback）。

== Robot Harness

#figure(
	image("../assets/ASENA/robot_harness.png", width: 100%),
	caption: [Robot Harness]
)<Robot_Harness>

@Robot_Harness 分为上下两部分：上方是系统结构，下方是机器人的技能。

=== 系统结构

上半部分从左到右是Coding Agent、ASENA和Physical robot，中间由四个箭头连接：

- Call：Agent写的程序调用ASENA提供的接口。
- Act：接口把调用转换为机器人的实际动作。
- Sense：机器人把传感器数据回传给接口。
- Inspect：Agent读取执行结果和传感器数据，检查程序有没有达到预期。

Call、Act、Sense，Inspect这四个Function构成一个闭环，Agent并不能一次性生成完整计划，需要根据看到的结果决定下一段程序怎么写。

+ Coding Agent框内给出了一个具体任务： Plan through an opening，图中的代码是论文压缩过的版本（condensed）：

	```python
	wall = p[obstacles & region, :2]
	counts, _ = np.histogram(wall[:,0], bins)
	floor = p[floor_mask & beyond_wall]
	```

	+ `p` 是LiDAR点云。用障碍物掩码和区域掩码筛出目标区域内属于墙的点，只保留 $(x, y)$ 平面坐标。
	+ 沿墙的方向对这些点做直方图统计。墙是连续的，每个区间都有很多点；如果某几个区间的点数明显偏少，说明那里没有墙，就是开口。
	+ 再取墙后方属于地面的点，确认开口后面确实有可以走的地面，而不是另一个障碍物。

	这里没有使用能力特化的模型，Agent只是用NumPy对原始点云做了几行统计，就从几何上推断出开口位置。即通过编程访问传感和计算：Agent遇到预定义技能没有覆盖的问题时，可以现场写代码解决。

	俯视的点云地图上，橙色折线是规划出的路线，标注了Opening的位置。Agent会读这张图来检查路线是否合理。右边的 Inside passage 是机器人执行后相机拍到的画面，证明它确实走进了通道。对应写程序 → 可视化检查 → 执行 → 用观测确认结果的过程。

+ ASENA Robot programming interface有三个模块：

	- Perception and planning：读取传感器、导航、抓取与运动规划，是Agent可以调用的能力。
	- Execution and feedback：下发指令、回报机器人状态和执行反馈，是Agent和机器人之间、计算机与物理世界之间的信息传输的途径。
	- Persistent workspace：保存程序、笔记和可复用技能，跨任务保留记忆，持久化提升Agent能力。

+ Physical robot：

	- 传感器：360°全景相机、LiDAR、Body IMU、Joint state、Odometry、Head RGB-D。
	- 执行器：SONIC（全身运动控制）、YAM（夹爪）、speaker。

=== 机器人的技能

Agent如何把感知、计算和执行组合起来：

+ Navigate passages：Estimate free space，traverse openings

	Wall geometry是从LiDAR中提取的墙体几何，Opening view是开口处的视角，Traverse和Pass through是机器人行走并穿过开口的画面。先用几何估计可通行空间，再穿过开口。

+ Inspect and localize：Choose views，verify the target

	Reorient是机器人调整朝向，Inspect是靠近观察；右边的Reference是参考物品（用户桌上的零食），Found item是在售货机里找到的同款。Agent自己决定从哪个角度看，并把看到的物品和参考物品比对来确认目标。

+ Plan a grasp：Grasp proposals → IK + checks

	先用RGB-D图像和SAM分割出目标物体，生成候选抓取，得到Selected grasp。然后通过三步流水线：Measured joints（读取实测关节角）→ Inverse kinematics（求解到达抓取位姿所需的关节角）→ Scene checks（检查与场景是否碰撞）。只有通过检查的抓取才会执行。

+ Grasp, lift, and carry：Grasp the object，transport it while walking

	Grasp、Lift、Carry三个画面依次是抓取、抬起和边走边搬运。人形机器人一边行走一边保持手臂稳定，需要SONIC全身控制器和夹爪控制同时工作。

+ Present and release：Extend the arm，open the gripper

	Present是伸出手臂把物体递到人面前，Support object是接收者的手托住物体，Release是张开夹爪松手。

+ Synthesize gestures

	Wave是挥手，Gesture yes是用肢体表示“是”。下方的流水线是：Code synthesis（Agent写代码生成动作）→ Online sim checks（在MuJoCo中在线验证）→ SONIC whole-body control（全身控制执行）。右侧的Motion trajectory曲线是手肘（elbow）和肩部（shoulder）关节角度随时间的变化，单位为弧度，持续约7.7 s。Agent现场写代码生成关节轨迹，经过仿真验证后再执行。

论文展示的这六类能力都是论文强调的Robot Harness，检查、抓取、搬运、递交和交流，都是Coding Agent通过详细的上下文：用户给出的任务+从各类传感器读取的数据与现实交互，然后使用同一套编程接口组合出来的。

== 任务流程

#figure(
	image("../assets/ASENA/data_stream.png", width: 100%),
	caption: [Data Stream]
)<data_stream>

@data_stream 展示的是系统的数据流，分为Coding Agent，ASENA，Physical robot三个部分。

=== Coding Agent

- Task + context：Agent的输入是任务描述加上下文，上下文包括工作区里已经积累的笔记和技能，所以同一个Agent在不同时期面对同一任务，拿到的上下文可能不同。
- Program robot interaction：Agent通过输出一段程序与机器人集成
- Write · Run · Inspect：Agent的工作循环，写程序、运行程序、检查结果。
- Read artifacts / Repair code：带折线的图标代表执行产物（日志、曲线、可视化），Agent读取这些产物，发现问题后修改代码。
- Fixed model weights：整个过程中Agent的模型权重不变，系统的学习完全发生在工作区里，而不是模型参数里。

=== ASENA

+ Workspace + feedback：Programs · Retained notes + skills

	持久化的工作区，保存程序、笔记和技能，Agent每次开始任务时首先查阅工作区，看有没有可以直接复用的东西。

+ Perception + callable tools

	Agent可以调用的工具，分为三种：
	- Geometric navigation · ASENA-VLN ：两种导航方式。几何导航基于LiDAR占据地图，ASENA-VLN是论文训练的学习型策略。
	- Segmentation → grasp proposals：分割物体后生成候选抓取。
	- IK · motion planning · scene checks：逆运动学、运动规划和场景检查。

	Agent通过函数调用使用这些工具。

+ Robot execution：Body motion · Grippers · Speech
	
	最终下发给机器人的三类指令：身体运动、夹爪开合和语音。

+ Execution record 

	MCAP bag格式的日志信息，包含三条按时间对齐的轨道：

	- Body + gripper state：机身与夹爪状态，图中画成一条连续的波动曲线，表示高频的状态信号。
	- Actions + feedback：动作与反馈，画成一串离散的方块，表示每次下发的指令和返回的结果。
	- Code + observations：代码与观测，画成一串文档图标，表示每个时刻运行的代码片段和对应的观测。

	Robot execution的每次执行都会被自动记下来。Agent也会持续读取这份记录。三条轨道上有一条竖直的虚线穿过，表示在同一时刻可以同时查到状态、动作和代码。这就是MCAP记录的作用：当执行出错时，Agent可以定位到出错的那一刻，看到当时机器人的状态、发出的指令和是哪一段代码发出的，从而找到要修改的程序。

=== Physical Robot

G1机器人上半部分是传感器：360° camera、Head RGB/stereo、LiDAR、Body IMU、Joint state、Odometry、Gripper state。下半部分是三个执行单元：SONIC负责身体运动，YAM负责夹爪，Speaker负责语音。

- sense（机器人 → Perception + callable tools）：传感器数据进入感知工具。
- act（Robot execution → 机器人）：指令下发到执行单元。
- state（机器人 → Execution record）：机器人的状态直接写入执行记录。

sense和state是两条不同的路径，sense给工具用，是实时的；state给记录用，是为了事后回放。

=== Experience Retention

+ Recorded evidence + task feedback：输入是MCAP执行记录和任务反馈。
+ Reflect with motion disabled：Agent对这些材料进行反思，此时机器人运动被禁用。反思阶段Agent可能会修改和测试代码，禁用运动保证了这些测试不会让机器人突然动起来。
+ Retain notes + skills：把反思得到的经验写成笔记，把修复好的程序整理成技能，存回模块1的工作区。

执行记录的产物最终回到了工作区，下一次任务开始时，Agent在 Task + context 中就能读到这些经验。

=== 两个闭环

综合来看，@data_stream 中有两个嵌套的闭环：

- 任务内闭环（实时）：calls → act → sense → Agent检查结果 → 再次calls。Agent在一次任务中边执行边观察，发现偏差就修改程序，所谓把程序建立在机器人反馈之上（Plan with Feedback）。
- 任务间闭环（离线）：执行 → MCAP记录 → inspect → 反思 → 写入工作区 → 下次任务读取。Agent在多次任务之间积累经验。把任务执行过程中的数据以及结果转化为保留下来的经验，实现工作区进化（Workstation Evolution）。

两个闭环都不更新模型权重：前者改的是当前程序，后者改的是工作区。

== Workspace Evolution

=== 执行记录

ASENA把每次运行保存为MCAP格式的记录，记录中把传感器和机器人状态与动作、反馈、代码、可视化在时间上对齐。Agent回放这些证据，就能定位执行在哪一步偏离了意图，并修复对应的程序。

=== 工作区

把第 $k$ 次修订的工作区记为：

$
	W_k = (N_k, S_k)
$

其中 $N_k$ 是笔记，$S_k$ 是可执行技能。权重固定的Coding Model在解决任务时参考这个工作区。两次修订之间，由另一个权重固定的Improver根据执行轨迹 $cal(T)_k$ 和反馈 $f_k$ 更新工作区：

$
	W_(k+1) = I_phi (W_k, cal(T)_k, f_k)
$

整个过程中模型和控制器的权重都不更新，进化的只有工作区里的文本和代码。

在仿真中，每一轮有16个Worker并行执行任务，每个Worker拿到共享工作区的一份私有副本，执行任务、查看反馈、创建或复用技能、记录经验。一轮结束后，Improver负责：

- 合并有用的可执行技能，并测试保留下来的代码；
- 跟踪每条内容的来源和效果；
- 删除冗余或无效的内容；
- 生成新的共享工作区供下一轮使用。

冻结评估时禁止写入工作区。在G1上，技能修改只能在机器人运动被禁用时，基于执行记录和操作员反馈进行。

#algorithm(
	[ASENA：工作区自进化],
	[任务集 $cal(D)$，初始工作区 $W_0$，进化轮数 $K$],
	[进化后的工作区 $W_K$],
	(
		(0, [for $k = 0, 1, ..., K - 1$ do]),
		(1, [parallel for 16 个 Worker do]),
		(2, [复制 $W_k$ 为私有工作区]),
		(2, [for 采样的任务 $tau in cal(D)$ do]),
		(3, [Coding Agent 读取任务和工作区，编写并执行程序]),
		(3, [保存 MCAP 执行记录], [状态、动作、代码对齐]),
		(3, [获取反馈 $f$，反思结果], [仿真中为真值反馈]),
		(3, [写入笔记，创建或修改技能]),
		(1, [收集所有 Worker 的轨迹 $cal(T)_k$ 与反馈 $f_k$]),
		(1, [$W_(k+1) <- I_phi (W_k, cal(T)_k, f_k)$], [合并、测试、去冗余]),
		(0, [return $W_K$]),
	),
)

== ASENA-VLN

#figure(
	image("../assets/ASENA/vln.png", width: 100%),
	caption: [Navigation policy architecture and atomic training examples]
)

ASENA-VLN是论文提出的一个导航工具，作用是减少Agent在困难路线上的重复推理：一次调用就执行多个运动决策，不需要Agent每走一步都重新观察和决策。Agent决定什么时候调用它、检查进度，并把它与其他工具组合使用。

=== 输入

模型基于Qwen3-VL-4B-Instruct，输入为最多8帧前视RGB图像和一条结构化JSON指令。视觉token按时间加权，保留时序上下文的同时突出最新的观测。指令包含目标文本和质量等级，可以带上可选的终点位姿锚点：

```json
{
  "goal": "Walk over to the built-in dishwasher off to your left.",
  "quality": 5,
  "end_pose": [{
    "target": "built-in dishwasher",
    "relation": "near",
    "distance_m": 0.55
  }]
}
```

其中：
- `goal`：导航指令文本，方向、路线提示和约束。
- `quality`：1–5的质量等级，训练时用来标注数据质量并作为条件输入，R2R/RxR评估时统一取5。
- `end_pose`：终点相对某个物体、房间或起点的关系和距离，由场景几何计算得到。例如房间目标用 `inside`，距离为0；“前进1米”用 `agent_start`、`forward`、`1.0`。

把指令统一成JSON格式，让这个策略可以像函数一样被Coding Agent调用。为了让原始数据集符合这个接口，论文用GPT-5.5把R2R、RxR等数据集的原始指令重新改写成这种结构化格式。

=== 输出

在Qwen3-VL原有词表上新增了1,343个token，共享同一个自回归解码器：

- Trajectory tokens：预测最多8个累积的机体坐标系航点，描述整条要走的路径，每个航点为 $(x, y, theta)$。每个轴被划分成固定的区间，token直接解码为明确的运动目标，控制器逐点跟踪即可。
- Pixel Goal tokens：输出 $(u, v, theta, "is_last")$，表示当前帧图像上的目标像素点，让模型在图像中定位它要去的地方，只在训练中作为辅助监督。
- 视觉问答：沿用原有的文本token。

Pixel Goal不能直接执行，需要额外的运动规划器（仿真里是最短路径跟随器SPF），所以论文所有评估都只使用轨迹token的预测。这一点和AGC-VLN、VLNontheFly不同，那两篇都是先预测图像中的目标点，再用几何投影得到世界坐标。

== 实验结果

=== 独立VLN策略

在R2R和RxR的val_unseen上单独评估ASENA-VLN，前进步长0.25 m，旋转15°：

#align(
	center,
	table(
		columns: (auto, auto, auto, auto, auto, auto),
		align: (left, center, center, center, center, center),
		table.header([方法], [R2R NE↓], [R2R SR↑], [R2R SPL↑], [RxR NE↓], [RxR SR↑]),
		[StreamVLN], [4.98], [56.9], [51.9], [6.22], [52.9],
		[DualVLN], [4.05], [64.3], [58.5], [4.58], [61.4],
		[Qwen-RobotNav-4B], [4.22], [66.9], [60.5], [4.15], [71.3],
		[ASENA-VLN-4B], [3.52], [68.7], [64.2], [3.90], [70.2],
	)
)

ASENA-VLN在R2R上各项指标领先，导航误差明显更低；RxR的SR略低于Qwen-RobotNav-4B，但NE和nDTW（73.1）更好。

在目标物体导航（ObjectNav）上，ASENA-VLN搭配一个单独的VLM（Gemini 3.1 Flash-Lite）负责规划去哪里搜索，在HM3D v2上达到80.8/38.1（SR/SPL），在HM3D-OVON的seen、同义词、unseen三类上分别为60.5、65.8、64.9的SR，均超过已发表方法，对未见过的类别也保持稳定。

=== Coding Agent与VLN策略的互补

在R2R和RxR各取100个任务构成Agentic Split，每个Agent都从空工作区开始：

#align(
	center,
	table(
		columns: (auto, auto, auto, auto, auto, auto),
		align: (left, center, center, center, center, center),
		table.header([方法], [VLN工具], [R2R SR↑], [R2R 调用次数↓], [RxR SR↑], [RxR 调用次数↓]),
		[Uni-LaViRA], [–], [60.7], [–], [51.3], [–],
		[ASENA-VLN-4B], [–], [78.0], [–], [62.0], [–],
		[ASENA (Sonnet 5)], [×], [61.0], [83.32], [50.0], [103.83],
		[ASENA (Sonnet 5)], [✓], [72.0], [33.96], [61.0], [38.91],
		[ASENA (GPT-6)], [×], [87.0], [26.37], [93.0], [40.80],
		[ASENA (GPT-6)], [✓], [88.0], [14.00], [86.0], [19.98],
	)
)

- 强Agent本身就是很强的零样本导航系统：GPT-6 Astra只用几何和编程工具，R2R上达到87% SR，接近人类水平90%，超过了单独的ASENA-VLN。
- VLN策略对弱Agent帮助更大：Claude Sonnet 5加上ASENA-VLN后，两个基准的SR都提高11个百分点。GPT-6 Astra受益较小，在RxR上开启VLN反而略有下降，说明学习型导航的价值取决于Agent能否有效利用策略的预测。
- VLN策略减少了重复交互：所有组合的平均工具调用次数都下降，GPT-6 Astra下降46.9%/51.0%，Claude Sonnet 5下降59.2%/62.5%。

=== Workstation Evolution

在同样的任务、场景、指令和初始位姿上重复执行10轮，每轮16个Claude Sonnet 5 Worker，使用真值反馈，所有权重固定：

- 四组实验（R2R/RxR，有/无VLN）的成功率都持续提升，使用VLN时R2R从72%到98%，RxR从65%到89%。
- 每个episode的花费随工作区进化总体下降。
- R2R上有无VLN最终成功率接近；RxR指令更长，VLN的优势更明显；有VLN的工作区在两个基准上SPL都更高。

需要注意，这个实验是在同一批任务上反复执行，衡量的是重复任务上的提升，不是对新任务的泛化。

=== Enbodied Question Answer

EQA要求Agent探索环境、收集视觉证据，并以尽量少的移动回答问题。Agent组合导航、视觉检查、物体搜索、地点记忆和回答生成工具。论文先手工设计了两个参考配置：quality（优先收集足够证据）和efficient（把探索步数当作有限资源）。之后从训练场景构建进化池（398个HM-EQA问题、1,183个MT-HM3D问题），每轮16个Worker各采样50个问题执行并反思，由Improver汇总更新工作区。

#align(
	center,
	table(
		columns: (auto, auto, auto, auto, auto),
		align: (left, center, center, center, center),
		table.header([方法], [HM-EQA Acc↑], [Steps↓], [MT-HM3D Acc↑], [Steps↓]),
		[Qwen-RobotNav], [76.7], [.15], [54.4], [.19],
		[ASENA base (low)], [70.9], [.15], [64.8], [.10],
		[ASENA 手工 (efficient)], [72.2], [.11], [63.8], [.11],
		[ASENA 手工 (quality)], [78.8], [.22], [67.1], [.24],
		[ASENA 进化17轮 (low)], [74.8], [.13], [64.9], [.08],
		[ASENA 进化17轮 (high)], [81.2], [.25], [65.5], [.22],
	)
)

进化后的工作区在低推理强度下，步数与手工efficient配置相当，准确率更高；HM-EQA上第17轮达到最高的81.2%。另外在132个MT-HM3D问题上的对照实验中，ASENA-VLN把探索步数中位数减少约19%，准确率基本不变。

== 局限性

- G1上的演示只是定性展示能力组合，没有在不同物体、布局和用户上系统地测量可靠性。
- 真实任务需要10–20分钟，推理和执行速度还需要提高才能实用。
- 工作区进化只在重复任务上验证过，保留的技能能否迁移到新任务还需要评估。
- 安全机制依赖操作员批准，对动态环境的安全保障还需加强。
- 目前只有工作区在进化，论文认为后续可以把学习型策略和Agent本身的更新也纳入进化。


#pagebreak()