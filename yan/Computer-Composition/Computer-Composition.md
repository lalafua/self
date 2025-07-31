## 考查目标
1. 掌握单处理器计算机系统中主要部件的工作原理、组成结构以及相互连接方式。
2. 掌握指令集体系结构的基本知识和基本实现方法，对计算机硬件相关问题进行分析，并能够对相关部件进行设计。
3. 理解计算机系统的整机概念，能够综合运用计算机组成的基本原理和基本方法，对高级编程语言（C 语言）程序中的相关问题进行分析，具备软硬件协同分析和设计能力。

### 绪论
```sh
# -E   -- preprocess only; do not compile, assemble or link
gcc -E hello.c -o hello.i 

# -S   -- compile only; do not assemble or link
gcc -S hello.c -o hello.s # asm file

# -c   -- compile and assemble, but do not link
gcc -c hello.c -o hello.o # binary file

gcc hello.c -o hello.out # executable file
```