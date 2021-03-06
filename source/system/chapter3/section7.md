## 3.7 分段

在一维的虚拟内存中，虚拟地址从0到最大地址，一个地址接着一个地址，如果出现多个表不停增长，可能会出现表增长而产生碰撞的情况。

因此我们需要一种不用特意管理表扩张和收缩的方法，这和虚拟内存解决程序段覆盖问题的方法相同。

我们使用的方法就是使用称为**段**的地址空间。每个段由一个从0到最大的线性地址序列构成，不同的段的长度可以不同。

由于每个段构成了一个独立的地址空间，所以它们可以独立地增长或缩小而不会影响到其他段。需要强调的是，段是一个逻辑实体，一个段可能包括一个过程、一个数组、一个堆栈、一个数值变量。

分段有利于几个方面：
1. 如果每个过程都位于一个独立的段中并且起始地址是0，那么单独编译好的过程连接起来的操作可以简化。可以直接对某一段的过程进行修改，而不会对其他过程产生影响。
2. 分段有助于几个进程之间共享数据和过程。

**分段是为了使程序和数据可以被划分为逻辑上独立的地址空间并且有助于共享和保护。**

### 3.7.1 纯分段的实现

分页和分段的实现本质上是不同的：页面是定长的而段不是。如果我们使用分页的淘汰策略用在分段上，就会产生碎片的情况。

### 3.7.2 分页与分段的结合

略。

段的大小会比页大，一般是一个段中有很多页。但是现在已经不怎么使用分段技术了。