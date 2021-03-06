## 4.0 概述
文件是进程创建的信息逻辑单元，一个磁盘一般含有成千上万个文件，每个文件是独立于其他文件的，唯一的不同是文件对磁盘的建模。

进程可以读取已存在的文件，并在需要的时候建立新的文件。存储在文件中的信息必须是持久的，也就是说，不会因为进程的创建与终止而受到影响。

文件是受操作系统管理的，有关文件的构造、命名、访问、使用、保护和管理都是操作系统所做的，总体上看，操作系统中处理文件的部分称为文件系统。

## 4.1 文件

文件是一种抽象机制，它提供了一种磁盘上保存信息并且方便以后读取的方法，这种方法可以使用户不必了解存储信息的方法、位置和实际磁盘工作方式等有关细节。

Win95和Win98用的是MS-DOS文件系统，即FAT-16，之后Win98扩展成为FAT-32，这两者是很相似的，但是过时了。之后的操作系统有了更先进的本地文件系统NTFS，该文件系统具有一些新的特性，例如基于Unicode编码的文件名。

### 4.1.1 文件名

许多操作系统支持文件名用圆点隔开分成两部分，圆点后面部分称为**文件扩展名**，前面是**文件名**。文件扩展名表示了文件的一些信息，在UNIX系统中一个文件甚至可以有两个或更多的扩展名。

并且在UNIX系统中，文件扩展名只是一种约定，操作系统并不强迫采用它。

### 4.1.2 文件结构

文件可以有多种构造方式，常见的三种方式如下：
- 字节序列
  
  把文件看成字节序列有非常大的灵活性，用户程序可以向文件中加入任何内容，并以任何方便的形式命名。操作系统不会提供任何帮助，也不会构成阻碍。所有UNIX和Windows都采用这种文件模型。

- 记录序列
  
  记录序列是文件结构的改进版本，在这个模型中，文件是具有固定长度记录的序列，每个记录都有内部结构，并且读操作返回一个记录，写操作重写或追加一个记录。

- 树
  
  文件结构由一颗记录树构成，每个记录不必有相同的长度，记录的固定位置有一个**键**字段，树是按照键字段排序。


### 4.1.3 文件类型

操作系统中文件类型分为以下几种：
1. 普通文件
2. 目录
3. 字符特殊文件
4. 块特殊文件

**普通文件**是包含用户信息的文件；**目录**是管理文件系统结构的系统文件；**字符特殊文件**和输入/输出有关，用于串行I/O设备，如终端、打印机、网络等；**块特殊文件**用于磁盘类设备。

普通文件一般又分为ASCII文件和二进制文件。ASCII文件最大的优势是可以显示和打印，可以用任何文本编辑器进行编辑。

和ASCII文件不同的是二进制文件，该文件只是一个字节序列，只有文件的格式正确时，操作系统才会执行这个文件。

二进制可执行文件在早期的UNIX中，该文件有5个段：
- 文件头
- 正文
- 数据
- 重定位
- 符号表

其中文件头以魔数开始，表明该文件是一个可执行文件，魔数后面是文件中各段的长度、执行的起始地址和标志位等。

### 4.1.4 文件访问

