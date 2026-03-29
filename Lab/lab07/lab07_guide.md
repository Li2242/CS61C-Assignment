# UC Berkeley CS61C Lab 07: Caches (缓存)

## 1. Experiment Overview & Goals (实验概述与目标)

欢迎来到 CS61C Lab 07！在本实验中，我们将深入探索计算机体系结构中最为关键的性能优化组件之一——**Cache（缓存）**。
你将通过模拟器观察缓存的底层工作原理，并亲自动手编写 C 代码，体验不同内存访问模式（Memory Access Patterns）如何显著影响程序的运行时间。

**本次实验的核心学习目标包括：**

1. 掌握缓存的关键参数（Cache Size, Block Size, Associativity）对 Hit/Miss Rate 的影响。
2. 学会使用 Venus 缓存模拟器分析 RISC-V 汇编代码的内存访问行为。
3. 深入理解**空间局部性（Spatial Locality）**和**时间局部性（Temporal Locality）**。
4. 掌握并实现**缓存分块(Cache Blocking / Tiling)**技术，以大幅提升矩阵运算等内存密集型任务的性能。

---

## 2. Prerequisite Knowledge (前置知识要求)

在开始动手之前，请确保你已经复习了相关 Lecture 内容，特别是以下几个核心概念：

* **Cache Hierarchy（缓存层次结构）:** CPU -> L1 Cache -> L2 Cache -> Main Memory。理解访问速度与容量的权衡。
* **The 3 C's of Cache Misses（三种缓存未命中类型）:**
  * **Compulsory (Cold) Miss:** 第一次访问某个数据块，必定发生 Miss。
  * **Capacity Miss:** 缓存太小，无法容纳程序在某段时间内需要的所有数据。
  * **Conflict Miss:** 由于多个内存块映射到同一个 Cache Set，导致相互驱逐（在 Direct-Mapped 或低相联度缓存中常见）。
* **Cache Mapping Policies（缓存映射策略）:**
  * **Direct-Mapped (直接映射):** 每个内存块只能放在一个特定的 Set 中。
  * **Fully Associative (全相联):** 内存块可以放在 Cache 的任何位置。
  * **N-Way Set Associative (N路组相联):** 结合前两者，每个 Set 有 N 个位置可以存放数据。
* **Cache Anatomy (缓存地址划分):**
  * 一个内存地址被划分为三部分：**Tag | Index | Offset**。
  * `Offset bits` = $\log_2(\text{Block Size in bytes})$
  * `Index bits` = $\log_2(\text{Number of Sets})$
  * `Tag bits` = $\text{Address Length} - \text{Index bits} - \text{Offset bits}$
* **C Array Memory Layout（C语言数组内存布局）:** C语言多维数组是以**行优先（Row-Major）**的方式在内存中连续存储的。`A[i][j]` 在物理内存中与 `A[i][j+1]` 紧挨着。
* **Cache Blocking (缓存分块):** 一种通过将大数据集分割成能装入缓存的小块（Blocks/Tiles），以最大化数据重用、减少 Cache Miss 的编程技术。

---

## 3. Setup & Environment (环境准备)

1. **代码拉取:** 确保你已经在本地（当前 `lab07` 目录）拥有所有 starter code。
2. **Venus 模拟器:** Task 1 需要使用基于 Web 的 Venus 模拟器。你可以打开 [Venus 网页端](https://venus.cs61c.org/) 并使用其内置的 Cache 工具。
3. **C 编译器:** Task 2 和 Task 3 会在本地运行 C 程序，确保系统中已安装 `gcc` 和 `make`。

---

## 4. Project Structure (文件结构说明)

在 `lab07` 目录下，你将看到以下文件，它们分别对应不同的实验任务：

* `cache.s`: 包含用于在 Venus 中模拟数组访问模式的 RISC-V 汇编代码（用于 Task 1）。
* `exercise1.txt`, `exercise2.txt`, `exercise3.txt`: 用于填写各任务对应思考题（Checkoff Questions）的文本文件。请在分析后填入你的答案和推理。
* `matrixMultiply.c`: 一段矩阵乘法的 C 源码，包含多种不同的循环嵌套顺序（用于 Task 2 观察性能差异）。
* `transpose.c`: 矩阵转置的实现文件，你需要在这里实现常规转置以及**缓存分块转置**（用于 Task 3）。
* `transpose.h`: 包含矩阵转置函数的声明。
* `test_transpose.c`: 用于测试你实现的转置函数正确性与性能的测试文件。
* `Makefile`: 包含用于一键编译和运行 C 代码的脚本配置。

---

## 5. Task 1: Cache Simulator (缓存模拟器)

### Implementation Details (实现细节)

你将使用 Venus 的缓存模拟功能来运行 `cache.s`。该汇编程序执行的操作是用特定的步长（Step Size）遍历一个数组，并将元素置零。你需要调整不同的缓存参数（如 Block Size, Cache Size, Associativity 等）和汇编程序的参数，观察并记录缓存命中率（Hit Rate）的变化。

### Step-by-step Instructions for Task 2 (分步操作)

1. **导入代码:** 在 Venus 中打开 `cache.s`。
2. **定位参数:** 注意 `main` 函数代码中的伪指令（如 `li a0, 256`），这代表了数组的大小、步长等参数。
    * `a0` = Array size in bytes.
    * `a1` = Step size in words (汇编中会将其乘以 4 转化为 bytes).
    * `a2` = Repetition count.
    * `a3` = Option (0 = write only, 1 = read and write).
3. **打开 Cache Simulator:** 在 Venus 界面的 "Simulator" 标签页下，找到并展开右侧的 "Cache" 工具。
4. **配置 Cache:** 根据 `exercise1.txt` 中的各个 Scenario，在 Cache 工具中设置对应的参数。
5. **运行并记录:** 运行代码，观察 Cache 工具提供的最终 Hit Rate 和 Miss 记录，分析原因并填入 `exercise1.txt`。

### Thinking Guide & Scenarios (思考指南)

#### Scenario 1: 基础情况

* **实验操作:** 设置对应的 Cache 参数（如 Direct-Mapped, 某特定大小和 Block Size），运行 `cache.s`。
* **思考方向:**
  * Block Size 如果是 8 Bytes，而一个 Word 是 4 Bytes。一个 Block 能存几个数组元素？
  * 每次访问一个全新的 Block 时，第一次必定是 Miss (Compulsory Miss)。如果步长 `a1=1`，紧接着访问同一 Block 中的下一个元素时，会命中吗？
  * 预测出 Hit Rate 应该是多少，并与模拟器结果对比。

#### Scenario 2: 改变循环步长 (Changing the Step Size)

* **实验操作:** 修改汇编代码中 `a1`（步长参数），将访问步长设为更大（例如跨越整个 Block 的大小）。
* **思考方向:**
  * 如果步长被设置得很大，每次访问都会跳过刚刚加载进 Cache Block 的相邻数据，直接访问下一个物理 Block 吗？
  * 此时的空间局部性（Spatial Locality）发生了什么变化？这对 Hit Rate 会产生什么毁灭性的打击？

#### Scenario 3: 改变相联度与缓存大小

* **实验操作:** 调整 Cache 配置为 2-Way 或 Fully Associative，或者改变 Cache 总大小。
* **思考方向:**
  * 在 Direct-Mapped 时，如果两个常用的数据块碰巧映射到同一个 Set，它们会发生严重的 Conflict Miss（Thrashing）。
  * 升级为 N-Way Set Associative 后，同一个 Set 可以容纳 N 个 Block。这能解决哪些类型的 Miss？

### Hints for Task 1 (关键提示)

* 永远把地址按 **Byte** 来计算。在 RISC-V 中，一个整数（word）占 4 Bytes。
* "步长为 1" 意味着每次地址递增 4 Bytes。"步长为 2" 意味着地址递增 8 Bytes。
* 计算命中率的基本公式：$Hit Rate = \frac{Hits}{Hits + Misses}$。如果是顺序访问，且一个 Block 能存 N 个元素，通常第一次 Miss，后面的 N-1 次都会 Hit（前提是没有被冲突驱逐）。

---

## 6. Task 2: Matrix Multiplication & Cache Access (矩阵乘法与内存访问模式)

### Specification (任务说明) — Matrix Multiplication

矩阵乘法 $C = A \times B$ 是科学计算的核心。常规的矩阵乘法需要三层嵌套循环（通常记为 `i`, `j`, `k`）。在这个任务中，你将编译并运行 `matrixMultiply.c`，观察只是改变这三个循环的**嵌套顺序**，程序运行时间为何会产生天壤之别。

### Step-by-step Instructions (分步操作)

1. 阅读 `matrixMultiply.c` 的源码，重点关注六个函数（例如 `multMat1` 对应 `i-j-k` 顺序，`multMat2` 对应 `i-k-j` 顺序，等等）。
2. 在终端运行编译命令：`make ex2` （或直接 `make matrixMultiply`）。
3. 执行程序：`./matrixMultiply`。
4. 程序会输出各种循环顺序下的执行速度（Gflops/s，即每秒十亿次浮点运算数，数值越高越快）。
5. 在 `exercise2.txt` 中记录你的发现，并回答哪种顺序最快，哪种最慢，以及**为什么**。

### Thinking Guide (思考指南)

* **C 语言的数组存储:** C 语言是以**行优先（Row-Major）**的方式存储二维数组的。这意味着遍历同一行的元素（改变列索引）在内存中是连续的。
* **空间局部性法则:** 沿着**行**遍历数组，会导致高度的空间局部性，因为你顺着内存地址逐个访问，缓存命中率极高（每次读取一个 Block 能带入好几个连续元素）。
* **反面教材（Strided Access）:** 沿着**列**遍历数组（固定列索引，改变行索引），由于矩阵很大，导致每次访问都跳跃一整行的距离（远超一个 Cache Block 的大小），产生海量的 Cache Misses。
* **具体分析矩阵乘法:** $C[i][j] += A[i][k] \times B[k][j]$。
  * 观察最内层循环变量：
  * 如果最内层是 `k`：对 $A[i][k]$ 而言，行固定、列变化，这是沿行访问（极好！）；对 $B[k][j]$ 而言，列固定、行变化，这是沿列访问（糟糕！）。
  * 如果最内层是 `j`：想一想 $A$（不依赖 $j$）、$B[k][j]$ 和 $C[i][j]$ 的访问模式会变成什么样？你能找到一个让所有数组都尽可能沿“行”访问的神奇顺序吗？

### Hints (关键提示)

* 你不必修改这部分的代码，重点在于**分析**。
* 画一个简单的内存图，模拟最内层循环步进一次时，各个数组的指针在内存里跳动了多远。跳得越远的（Stride 很大），性能越差。

---

## 7. Task 3: Cache Blocking (缓存分块实战)

### Specification (任务说明) — Matrix Transpose

矩阵转置（Matrix Transpose）操作：$B[j][i] = A[i][j]$。
如果是按行顺序遍历 $A$，则意味着对 $B$ 是按列跨步访问，反之亦然。对于大矩阵，这会导致缓存疯狂抖动（Thrashing）。
你的终极任务是修改 `transpose.c`，实现一个**带有缓存分块（Cache Blocking）**的转置函数 `transpose_blocking`。并在 `exercise3.txt` 中记录不同参数下的性能差异。

### Thinking Guide: What is Cache Blocking? (原理剖析)

* **常规转置的痛点:** 如果矩阵非常大，读一行 $A$ 会填满 Cache，但写 $B$ 时要跨越海量地址。等你要处理 $A$ 的下一行时，$B$ 中之前缓存的相关列可能早就被踢出 Cache 了。这浪费了大量的时间和空间局部性。
* **分块技术（Blocking/Tiling）的魔法:** 我们不一次性处理整行或整列，而是将大矩阵划分为一个个边长为 `blocksize` 的小方块（子矩阵）。
  * 只要确保这个 `blocksize * blocksize` 的小方块能完整塞进 CPU 的 L1/L2 Cache 中，我们在处理这个小方块时，所有的读写都会在极高速的 Cache 中命中！
  * 处理完当前块，再移动到下一个块。

### Implementation Steps (实施步骤)

1. **打开文件:** 编辑 `transpose.c`。
2. **构思 `transpose_blocking` 的框架:**
    * 你需要**四层**嵌套循环。
    * **外侧两层循环（块循环）:** 用于遍历矩阵中各个“块”的左上角坐标。它们步进的值是 `blocksize`。
        * 例: `for (int block_x = 0; block_x < n; block_x += blocksize) { ... }`
    * **内侧两层循环（元素循环）:** 用于遍历当前“块”内部的每一个元素。它们进行实际的 $dst[y + x \cdot n] = src[x + y \cdot n]$ 操作。
        * 例: `for (int x = block_x; x < min(block_x + blocksize, n); x++) { ... }`
3. **处理边缘情况 (Edge Cases 极度重要!):**
    * 注意矩阵的边长 `n` **不一定**能被 `blocksize` 完美整除！
    * 这意味着在矩阵的边缘，最后剩下的块可能是一个残缺的矩形，而不是完整的正方形。
    * **Hint:** 在内层循环设定边界时，千万不要直接写 `x < block_x + blocksize`，否则会引发越界（Segfault）。应该在内层循环的终止条件里做判断，严格防止数组越界读写。你可以使用类似 `(block_x + blocksize < n) ? (block_x + blocksize) : n` 的逻辑。
4. **填补代码并优化逻辑。** 不要修改外层提供的函数签名。

### Testing & Running (编译与测试)

1. 在终端执行：`make ex3` （或者 `make transpose`，具体参考 Makefile）。
2. 运行程序：`./test_transpose`
3. 该测试程序会测试各种大小的 `n` 和 `blocksize`。
4. **性能观测:** 观察终端输出，比较 `Naive` 和 `Blocking` 的运行时间差异。完成 `exercise3.txt` 中要求填写的时间数据。
    * 在小矩阵下，两者差距可能不大。
    * 但在大矩阵（如 `n=10000`）下，找到合适的 `blocksize` 会让 Blocking 的优势呈现碾压态势！寻找能够让时间大幅降低的那个 "Sweet Spot"（最佳 `blocksize`）。

### Common Mistakes & Fixes (常见错误与调试排雷)

* **Segmentation Fault (段错误):**
  * *原因:* 99% 都是因为没有正确处理边缘情况，导致分块内层循环访问超出了矩阵边界 `n`。
  * *解法:* 仔细检查最内侧两层循环的结束条件，确保坐标索引永远严格 `< n`。
* **结果验证失败（Validation Failed）:**
  * *原因:* 转置逻辑写反了，或者内层循环的索引变量弄混了。`x` 和 `y` 的对应关系出错。
  * *解法:* 可以在草稿纸上画一个 5x5 矩阵，设 `blocksize=2`，手动走一遍你的 4 层循环逻辑，看访问的一维坐标算式 `[x + y * n]` 是否正确反映了二维位置。
* **性能没有提升反而下降:**
  * *原因:* 你的分块最内层循环顺序依然导致了极其糟糕的 Cache Miss。
  * *解法:* 检查内层元素级循环遍历时的连续性。分块本身能解决大部分问题，但内层循环的顺序依旧值得推敲。

---

## 8. Key Takeaways (核心总结)

1. **缓存至上:** 在现代体系结构中，CPU 运算极其迅速，**内存访问速度（Memory Wall）**才是真正的性能瓶颈。
2. **顺势而为 (Memory Access Patterns):** 你的代码如果能顺应硬件的特性（优先按行访问顺序排列的数组），就能白嫖硬件提供的空间局部性红利。
3. **化整为零 (Blocking/Tiling):** 面对超过缓存容量的庞然大物，通过算法将其切割为适应 Cache 尺寸的小块，是突破性能极限的高阶武器。

现在，带着这些思路，去征服那些 Cache Miss 吧！Happy Coding! 🐻
