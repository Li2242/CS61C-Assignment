# UC Berkeley CS61C Lab 06 实验指南（RISC-V Assembly & Processor Fundamentals）

> 说明：本指南基于你当前 `lab06` starter code（`ex1.circ` + `ROMdata`）编写，遵循 CS61C 实验风格。  
> 本文提供完整学习路径、思考框架、调试方法与检查标准，**不提供完整电路答案**。

---

## Experiment Overview & Goals

本实验通过一个对比场景理解“处理器流水线基础思想”：

- 给定相同输入流，比较 **non-pipelined** 与 **pipelined** 两种数据路径。
- 在 Logisim 中完成电路连接，观察吞吐与时序行为差异。

从 starter 可见：

- 主电路 `main` 已搭建输入驱动与结果观测框架。
- `non_pipelined` 子电路已实现。
- `pipelined` 子电路是待完成核心。

完成本实验后你应能：

- 解释为什么流水线能提升吞吐（而不一定降低单条延迟）。
- 识别并正确使用寄存器进行阶段切分。
- 在 Logisim 中进行时钟驱动、ROM 输入序列喂入、波形/探针观察与结果验证。

---

## Prerequisite Knowledge（对应 Lecture）

建议先复习这些内容：

- 组合逻辑与时序逻辑的本质差异。
- 寄存器在时钟边沿采样中的作用。
- Pipeline 基本概念：stage、throughput、latency。
- 结构化电路分层：主电路 + 子电路输入输出契约。
- Logisim 常用组件：Pin、Adder、Multiplier、Register、Counter、ROM、Clock、Probe。

对应 CS61C 讲义关键词：

- Datapath Basics
- Pipelining Motivation
- Sequential Circuits
- Cycle-by-Cycle Observation

---

## Setup & Environment

### 1. 文件准备

`lab06` 目录包含：

- `ex1.circ`
- `ROMdata`

### 2. 打开电路

- 使用课程指定 Logisim-evolution 版本打开 `ex1.circ`。
- 确认主电路里可见：
  - Address Counter,
  - ROM,
  - `non_pipelined` 与 `pipelined` 两个子电路实例,
  - 输出 `non_pipe_out` 与 `pipe_out`。

### 3. 基本操作建议

- 先不改任何连线，直接用时钟单步 Tick 观察现有行为。
- 每改一次 `pipelined`，都从复位/初始状态重新走几拍看输出轨迹。

---

## Project Structure（所有文件用途）

### `lab06/ex1.circ`

包含三个关键电路：

1. `main`
   - 负责输入激励与结果对比。
   - Counter 输出地址，ROM 根据地址输出测试数据。
   - 数据分发给 `non_pipelined` 与 `pipelined`。
   - `Probe` + 输出 Pin 用于观察两路结果。

2. `non_pipelined`
   - 已完成参考路径。
   - 从连线可见核心运算模式为：先组合运算，再进入寄存器反馈。
   - 可作为你设计 `pipelined` 的功能基准。

3. `pipelined`
   - 当前几乎只有输入输出 Pin，待你完成。
   - 目标是与 `non_pipelined` 对应功能一致，但具备流水级切分。

### `lab06/ROMdata`

- ROM 原始数据文件，内容为测试输入序列。
- 用于驱动主电路在每个地址输出不同输入模式。

---

## Task 1: 读懂 `main` 测试框架

### Specification

在动手搭建 `pipelined` 前，必须先确认：

- 输入来自哪里；
- 时钟如何推进样例；
- 两个子电路输出怎样被并列观测。

### Thinking Guide

主电路的价值是“统一驱动 + 并行对比”。如果你不先读懂它，后续很容易把功能错误误判为时序错误。

### Hints

- 跟踪线网：Counter -> ROM -> 两子电路输入。
- 看 `Probe` 所在位置，理解当前观测点是在寄存器前还是后。
- 注意所有关键时序元件是否共享同一个 `CLK`。

### Implementation Steps

1. 打开 `main`，从 `Clock` 开始顺线到 Counter。
2. 查看 ROM 内容是否加载为 `ROMdata` 对应值序列。
3. 标记进入 `non_pipelined` 与 `pipelined` 的输入位宽与含义。
4. 单步 Tick 数次，记录 `non_pipe_out` 的更新节奏。
5. 记下“第几拍开始出现稳定输出”，作为后续对照基线。

---

## Task 2: 分析 `non_pipelined` 的等价行为

### Specification

你需要从已给电路反推功能关系（只需行为层面，不必重画）。

### Thinking Guide

从 `non_pipelined` 可读出一个“组合 + 状态反馈”的单周期路径。你应明确：

- 输入 `in1/in2` 先经过哪些组合运算；
- 哪个寄存器保存状态；
- 输出 `out1` 是否直接受寄存器当前值影响。

### Hints

- 特别关注 Adder、Multiplier 与 Register 的连接顺序。
- 观察反馈线是否形成“上一拍结果参与下一拍计算”的结构。

### Implementation Steps

1. 画出简化框图（纸上即可）：输入、组合块、寄存器、反馈、输出。
2. 用 2~3 组手工输入，按拍推演寄存器值变化。
3. 在 Logisim 单步验证你的推演是否一致。
4. 把该行为定义为 `pipelined` 必须保持的功能目标。

---

## Task 3: 在 `pipelined` 子电路实现分级数据路径

### Specification

实现目标：

- `pipelined` 与 `non_pipelined` 在稳态功能上对齐（给定同序列输入，输出关系应可解释地对应）。
- 使用寄存器将长组合路径拆分为多级。
- 所有时序元件由同一 `CLK` 驱动。

### Thinking Guide

流水线设计核心不是“多放寄存器”，而是“在合理边界切分组合逻辑并保证数据对齐”。

你需要明确三件事：

1. 哪些组合运算放在第 1 级，哪些放在第 2 级。
2. 每一级寄存器要保存哪些中间量。
3. 若不同路径延迟拍数不同，如何做对齐（必要时添加平衡寄存器）。

### Hints

- 不要改变输入输出 Pin 名称、方向和位宽。
- 每加一个阶段，先验证局部中间信号，再看最终输出。
- 如果输出“数值正确但拍数错位”，通常是流水级对齐问题，不是功能方程错误。
- 如果输出始终 `U`，优先检查是否有未驱动输入/寄存器数据端。

### Implementation Steps

1. 根据 Task 2 的功能框图，选定切分点。
2. 在切分点后放置阶段寄存器，并连接 `CLK`。
3. 确保每个阶段输入来自“上一阶段寄存结果”或本拍外部输入。
4. 对可能旁路到后级的信号做拍数对齐。
5. 连接最终 `out1`，确保位宽与主电路一致。
6. 回到 `main`，单步 Tick 观察 `pipe_out` 与 `non_pipe_out` 的关系。

---

## Task 4: 功能一致性与时序差异验证

### Specification

你需要证明两件事：

1. 功能上：`pipelined` 计算逻辑没有偏离目标。
2. 时序上：你能解释 pipeline fill/drain 导致的前几拍差异。

### Thinking Guide

流水线常见观察结果：

- 前若干拍 `pipe_out` 可能尚未产出有效目标值（管线填充）。
- 稳态后以固定节奏输出结果。

### Hints

- 对比时不要只看某一拍，要看连续多拍轨迹。
- 可在关键中间节点加临时 `Probe`，定位是“功能错”还是“对齐错”。

### Implementation Steps

1. 在 `main` 连续 Tick 至少一个完整输入序列。
2. 记录每拍的 `non_pipe_out` 与 `pipe_out`。
3. 标出管线填充区间与稳态区间。
4. 检查稳态阶段两者是否满足预期对应关系。
5. 若不一致，返回 `pipelined` 做局部修正再复测。

---

## Testing、Running、Debugging

### 1) 运行与观察

- 使用 `Tick Once` 做逐拍调试。
- 必要时 `Ticks Enabled` 连续运行后暂停查看。

### 2) 调试优先级

1. 先消除 `U`/红线（连线与位宽问题）。
2. 再确认各级寄存器都接到同一时钟。
3. 再验证阶段内组合逻辑是否正确。
4. 最后处理跨阶段拍数对齐。

### 3) 建议观察点

- `pipelined` 每一级寄存器输出。
- 最终 `pipe_out`。
- 与 `non_pipe_out` 的逐拍对照。

---

## Common Mistakes & Fixes

### Mistake A：只复制非流水线路径，未真正分级

- 现象：行为几乎与 non-pipeline 同拍，达不到实验目的。
- 修复：在明确切分点后加入阶段寄存器并重连数据路径。

### Mistake B：寄存器时钟漏接或接错

- 现象：某阶段数据不更新，输出卡死或 `U`。
- 修复：统一检查所有寄存器 `CLK` 连接。

### Mistake C：位宽不一致

- 现象：红线/蓝线、结果截断或扩展异常。
- 修复：统一 Pin、Adder、Multiplier、Register 的数据位宽。

### Mistake D：跨级信号未对齐

- 现象：数值看起来“像对的”，但总滞后/超前一拍或多拍。
- 修复：对旁路路径加平衡寄存器，确保同一拍参与运算的数据属于同一“指令/样本”。

### Mistake E：把填充期输出当成错误

- 现象：前几拍不一致就误判失败。
- 修复：区分管线填充期与稳态期，再做功能比较。

---

## Key Takeaways

- Pipeline 的核心收益是吞吐提升，代价是更复杂的时序管理。
- 寄存器不仅“存值”，更是阶段边界与时间对齐工具。
- 电路调试要按层次进行：连线正确 -> 局部正确 -> 全局时序正确。
- 对比 non-pipelined 参考实现是最有效的正确性锚点。

---

## 提交前 Checklist

- `pipelined` 子电路无悬空、无位宽冲突。
- 时钟连接完整一致。
- 逐拍观察中可解释 fill/drain 与稳态行为。
- 稳态功能输出与目标行为一致。
- 你可以口头说明每一级寄存器保存什么、为什么这样切分。
