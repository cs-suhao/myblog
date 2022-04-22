## 1.5 操作系统概念

操作系统需要使用一些基本概念和抽象，如进程、地址空间以及文件，这里先简要介绍，后面章节会详细说明。

### 1.5.1 进程

进程的本质是正在执行的一个程序。[详情见第二章]()

### 1.5.2 地址空间

[详情见第三章]()
地址空间是从某一个最小值的存储位置，到某个最大值的存储位置的列表。在这个地址空间中，进程可以进行读写，该地址空间中存放有：
- 可执行程序
- 程序数据
- 程序堆栈
- 程序计数器
- 堆栈指针

### 1.5.3 文件

支持操作系统的另一个关键概念是文件系统，目录层结构的每一个文件都可以通过从目录的顶部即根目录开始的路径名确定。绝对路径包含了从根目标路到该文件的所有目录清单，之间用正斜杠隔开。[详情见第四章]()

### 1.5.4 输入/输出

略。

### 1.5.5 保护

UNIX操作系统通常对每个文件赋予一个9位的二进制保护代码，该保护代码有三个3位字段，分别是：
- 所有者
- 与所有者同组的其他成员
- 其他人

3位字段分别是`r`读，`w`写，`x`执行。
### 1.5.6 shell

UNIX的命令解释器称为`shell`，虽然shel不是操作系统的一部分，但是体现了许多操作系统的特性。

shell以终端作为标准输入和标准输出，当用户键入命令，shell将运行程序作为子进程，直到子进程结束。