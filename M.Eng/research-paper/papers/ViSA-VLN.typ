#import "../lib/common.typ"

= ViSA-VLN

Paper: #link("https://arxiv.org/abs/2603.08007")[ViSA-Enhanced Aerial VLN: A Visual-Spatial Reasoning Enhanced Framework for Aerial Vision-Language Navigation]

VPG->VM->Executor

#image("../assets/ViSA-VLN/architecture.png")

== Methods

=== Visual Prompt Generator(VPG)

输入：原始鸟瞰图像 I_t
输出：带编号标注的结构化视觉表示

+ 开放词表检测：直接调用VLM原生的检测能力，用自由形式的自然语言提示一次性定位多个目标。这样支持复杂提示、上下文感知。
+ 划分区域、叠加Set-of-Mark(SoM)标注：把图像划分为不同粒度的区域并打上 Set-of-Mark 编号，每个编号唯一指向一个物理实体。

=== Verification Phase(VM)

+ 字面属性匹配：通过引用 SoM 标识符（例如，1），验证模块核实候选目标的可见特征是否与自然语言指令一致。它严格遵循字面观察原则；验证模块仅评估直接可见属性，并通过以下方式暂缓决策：在视觉证据仍不充分时，将目标标记为“待定”。
+ 空间拓扑验证：验证模块通过引用数字标识符（例如，“1在2的对面”）来验证空间关系，而非依赖离散文本场景图，从而消除指称歧义。
+ 地理边界验证：验证模块通过检查候选对象是否符合与已知地标之间的空间关系，确保其在地理上合理，避免选中视觉上匹配但地理位置不正确的目标。

迭代式 VPG 提示：当 VM 判断证据不足时，会输出自然语言引导信号 $g$（如 "focus on white vehicles near the intersection"）回传给 VPG，作为下一轮检测的文本提示，形成感知→验证的闭环反馈（图 1 里 VM→VPG 的那条回边）。

#image("../assets/ViSA-VLN/reseasoning_trace.png")

#pagebreak()


