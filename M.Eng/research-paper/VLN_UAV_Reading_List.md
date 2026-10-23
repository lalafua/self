# VLN / VLM / 无人机导航 阅读清单

★ = 建议精读。arXiv 编号已核对（2026-10）。

## 1. 综述
- ★ [VLN: A Survey of Tasks, Methods, and Future Directions](https://arxiv.org/abs/2203.12667) (ACL 2022)
- ★ [VLN Today and Tomorrow: A Survey in the Era of Foundation Models](https://arxiv.org/abs/2407.07035)
- [UAVs Meet LLMs: Overviews and Perspectives Toward Agentic Low-Altitude Mobility](https://arxiv.org/abs/2501.02341)

## 2. VLN 经典基础
- ★ [Vision-and-Language Navigation (R2R)](https://arxiv.org/abs/1711.07280) (CVPR 2018)
- [Speaker-Follower Models](https://arxiv.org/abs/1806.02724) (NeurIPS 2018)
- [EnvDrop: Back Translation with Environmental Dropout](https://arxiv.org/abs/1904.04195) (NAACL 2019)
- ★ [Beyond the Nav-Graph: VLN-CE](https://arxiv.org/abs/2004.02857) (ECCV 2020)
- [Room-Across-Room (RxR)](https://arxiv.org/abs/2010.07954) (EMNLP 2020)
- [REVERIE](https://arxiv.org/abs/1904.10151) (CVPR 2020)

## 3. Transformer / 预训练时代
- [VLN↻BERT](https://arxiv.org/abs/2011.13922) (CVPR 2021)
- [HAMT](https://arxiv.org/abs/2110.13309) (NeurIPS 2021)
- ★ [DUET: Dual-scale Graph Transformer](https://arxiv.org/abs/2202.11742) (CVPR 2022)
- [BEVBert](https://arxiv.org/abs/2212.04385) (ICCV 2023)
- [ETPNav](https://arxiv.org/abs/2304.03047) (TPAMI 2024)
- [ScaleVLN: Scaling Data Generation in VLN](https://arxiv.org/abs/2307.15644) (ICCV 2023)

## 4. LLM/VLM 驱动的导航
零样本 / 推理类：
- [LM-Nav](https://arxiv.org/abs/2207.04429) (CoRL 2022)
- [VLMaps](https://arxiv.org/abs/2210.05714) (ICRA 2023)
- ★ [NavGPT](https://arxiv.org/abs/2305.16986) (AAAI 2024)
- [NavGPT-2](https://arxiv.org/abs/2407.12366) (ECCV 2024)
- ★ [MapGPT](https://arxiv.org/abs/2401.07314) (ACL 2024)
- [VLFM](https://arxiv.org/abs/2312.03275) (ICRA 2024)

端到端 VLM 导航：
- ★ [NaVid](https://arxiv.org/abs/2402.15852) (RSS 2024)
- [Uni-NaVid](https://arxiv.org/abs/2412.06224) (RSS 2025)
- [NaVILA](https://arxiv.org/abs/2412.04453) (RSS 2025)
- [StreamVLN](https://arxiv.org/abs/2507.05240)

## 5. 无人机 VLN（核心）
- ★ [AerialVLN](https://arxiv.org/abs/2308.06735) (ICCV 2023)
- ★ [Aerial Vision-and-Dialog Navigation (AVDN)](https://arxiv.org/abs/2205.12219) (ACL Findings 2023)
- ★ [CityNav](https://arxiv.org/abs/2406.14240)
- ★ [Towards Realistic UAV VLN: Platform, Benchmark, and Methodology (OpenUAV)](https://arxiv.org/abs/2410.07087) (ICLR 2025)
- [Exploring Spatial Representation to Enhance LLM Reasoning in Aerial VLN](https://arxiv.org/abs/2410.08500)
- ★ [CityNavAgent](https://arxiv.org/abs/2505.05622) (ACL 2025)
- ★ [OpenFly](https://arxiv.org/abs/2502.18041)
- ★ [See, Point, Fly (SPF)](https://arxiv.org/abs/2509.22653)
- [UAV-ON: Open-World Object Goal Navigation with Aerial Agents](https://arxiv.org/abs/2508.00288)

## 6. VLM / 具身基础模型
- [CLIP](https://arxiv.org/abs/2103.00020)
- [BLIP-2](https://arxiv.org/abs/2301.12597)
- ★ [LLaVA: Visual Instruction Tuning](https://arxiv.org/abs/2304.08485)
- [Qwen2.5-VL Technical Report](https://arxiv.org/abs/2502.13923)
- [RT-2](https://arxiv.org/abs/2307.15818)
- [OpenVLA](https://arxiv.org/abs/2406.09246)
- [ChatGPT for Robotics](https://arxiv.org/abs/2306.17582)

## 阅读路线
1. 第 1 节两篇 ★ 综述
2. R2R → VLN-CE → DUET → NavGPT → NaVid
3. 精读第 5 节全部论文
4. 从 AerialVLN 的 Semantic Scholar 被引列表追踪最新工作
