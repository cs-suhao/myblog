## 2.3 进程间通信

进程间的通信主要有三个问题：
1. 一个进程如何把消息传递给另一个进程
2. 两个或更多的进程如何确保在关键活动不会出现交叉（例如定座位票）
3. 进程间如何保证运行的顺序

需要注意的是，这几个问题同样适用于线程。

### 2.3.1 竞争条件

**竞争条件**在多核增长的并行环境非常常见，由于多个进程的协作可能会共享一些彼此都能读写的公用存储区。竞争条件指的是：**两个或多个进程读写某些共享数据，而最后的结果取决于进程运行的精确时序**。

例如进程A修改变量a令其+1，进程B也需要修改变量a令其+1，如果A先执行，得到的结果是2，B得到的是3.而如果A和B时序变换，得到的结果会产生变化。这就是一种竞争条件。

### 2.3.2 临界区

如何避免竞争条件？关键在于找出某种途径来组织多个进程同时读写共享的数据。我们把对共享内存进行访问的程序片段称为**临界区域**或**临界区**。

我们需要是的两个进程不能同时处于临界区，就能避免竞争条件。除此之外，还需要满足以下4个条件：
1. 任何两个进程不能同时处于其临界区。
2. 不应该对CPU的速度和数量做任何假设。
3. 临界区外运行的进程不得阻塞其他进程。
4. 不得使进程无期限等待进入临界区。

### 2.3.3 忙等待的互斥

现有的几种实现互斥的方案如下：

#### 2.3.3.1 屏蔽中断

在单处理系统中，最简单的方法是使每个进程刚刚进入临界区后立即屏蔽所有中断，并在就要离开之前再打开中断。在屏蔽中断后，时钟中断也会被屏蔽，因为CPU只有发生时钟中断或其他中断才会进行进程切换。

但是这个方案并不好，因为把屏蔽中断的权力交给用户进程是不明智的。有可能一个进程屏蔽中断以后不再打开，会造成系统的终止。并且，如果多处理器中，只屏蔽单个CPU，其他CPU一样可以对共享变量进行访问。

#### 2.3.3.2 锁变量

第二种尝试是软件解决方法，设想有一个共享变量(锁)，初始值为0。当一个进程进入临界区，首先测试这把锁，如果该锁的值为0，则该进程将其设置为1并进入临界区。如果这把锁的值为1，则该进程等待直到锁的值编程0。

但是这种方法仍然有漏洞，假设一个进程读出锁变量的值为0，而恰好在其将锁置为1前，另一个进程被调度执行，将锁置为1。之后第一个进程也会将锁置为1，并且此时有两个进程同时进入临界区。

#### 2.3.3.3 严格轮转法

第三种尝试代码如下：
```c
// Process A
while(1){
    while(turn!=0);
    critical_region();
    turn = 1;
    noncritical_region();
}

// Process B
while(1){
    while(turn!=1);
    critical_region();
    turn = 0;
    noncritical_region();
}
```

这里的`turn`变量用于记录轮到哪一个进程进入临界区。开始时，进程0检查turn，发现turn的值为0，进程0进入临界区。进程1发现turn值不为1，因此一直等待。

连续测试一个变量直到某个值出现为止，称为**忙等待**。由于这种方式浪费CPU事件，所以通常认为等待时间非常短的情况下，才使用忙等待，而用于忙等待的锁称为**自旋锁**。

但是会出现这样的情况：进程0执行完整个循环，会将turn置为1，而进程1虽然退出临界区，但是没有完成整个非临界区的任务，因此没有将turn置为0。也就是说，进程0比进程1快，进程0都执行两轮了，进程1一轮还没有完成。

这时就会导致，进程0开始新一轮的循环时，需要等待进程1完成非临界区程序，因此违反了第三个条件，这个方案也不够好。

#### 2.3.3.4 Peterson解法

Peterson是一种简单的多的互斥算法，代码如下：
```c
#define FALSE 0
#define TRUE  1
#define N     2

int turn;
int interested[N];

void enter_region(int process){
    int other;
    other = 1-process;
    interested[process] = TRUE;
    turn = process;
    while(turn == process && interested[other] == TRUE);
}

void leave_region(int process){
    interested[process] = FALSE;
}

```

在进入临界区前，各个进程使用其进程号0或1作为参数调用`enter_region`，该调用在需要时将使进程等待。即使两个进程同时调用`enter_region`，也会因为`turn`变量被重写导致只有一个进程可以进入，另一个进程会因为满足while的循环条件一直等待。

#### 2.3.3.5 TSL指令

这是一种需要硬件支持的方案。某些计算机中，特别是哪些设计为多处理器的计算机，都有下面一条指令：`TSL RX, LOCK`。称为**测试并加锁**(test and set lock)，它将一个内存字lock读到寄存器RX中，然后在该内存地址上有一个非零值。读和写的操作保证是不可分割的，即该指令结束之前其他处理器均不允许访问该内存字。执行TSL指令的CPU将锁住内存总线，防止其他CPU在本指令结束前访问。


### 2.3.4 睡眠与唤醒

Peterson解法和TSL解法都是正确的，但是有忙等待的缺点，两个方案的本质都是：当一个进程想进入临界区，先检查是否允许进入，若不允许，则该进程将原地等待，直到允许为止。

但是这两个方案除了浪费CPU资源外，还会产生其他错误。考虑到两个优先级不同的进程L和进程H，调度规则规定只要H处于就绪态就可以运行，但是某一时刻L处于临界区，H准备运行。但是由于H处于忙等待，而L又不会被调度，那么H将会一直等待下去，会产生**优先级反转问题**。

有一些进程间通信原语，可以在无法进入临界区时阻塞，而不是忙等待。分别是`sleep`和`wakeup`。

#### 2.3.4.1 生产者-消费者问题

我们考虑**生产者-消费者问题**，也称为**有界缓冲区问题**。两个进程共享一个公共的固定大小的缓冲区，其中一个是生产者，将信息放入缓冲区，另一个是消费者，从缓冲区中取出信息。

含有严重竞争条件的生产者-消费者问题代码如下：

```c
#define N 100
int count = 0;

void producer(){
    init item;
    while(TRUE){
        item = produce_item();
        if(count == N) sleep();
        insert_item(item);
        count = count + 1;
        if(count == 1) wakeup(consumer);
    }
}

void consumer(){
    init item;
    while(TRUE){
        if(count == 0) sleep();
        item = remove_item();
        count = count - 1;
        if(count == N-1) wakeup(producer);
        consume_item(item);
    }
}

```

如果我们在消费者刚刚读取`count`值就调度生产者，生产者使用`wakeup`但是消费者并没有真正睡眠，等调度回消费者，消费者开始睡眠，会导致消费者再也醒不过来。

### 2.3.5 信号量

信号量使用一个整型变量累积唤醒次数，取值可以为0(表示没有保存下来的唤醒操作)或者为正值(表示有一个或多个唤醒操作)。对于信号量有两种操作:
- `up`
- `down`

对信号量执行`down`操作，检查其值是否大于0，若大于0则减1(用掉一个保存的唤醒信号)并继续，若该值为0，则将进程睡眠，并且此时down操作并没有结束。

对信号量执行`up`操作，对信号量的值加1。如果一个或多个进程在该信号量上睡眠，无法完成一个先前的down操作，则由系统选择其中一个并允许改进成完成down操作。


**值注意的是，这两个操作都是原子操作，也就是说检查数值、修改变量以及可能发生的睡眠操作都是不可分割的**。

#### 2.3.5.1 用信号量解决生产者-消费者问题

```c
#define N 100
typedef int semaphore;
semaphore mutex = 1;
semaphore empty = N;
semaphore full = 0;

void producer(){
    init item;
    while(TRUE){
        item = produce_item();
        down(&empty);
        down(&mutex);
        insert_item(item);
        up(&mutex);
        up(&full);
    }
}

void consumer(){
    init item;
    while(TRUE){
        down(&full);
        down(&mutex);
        item = remove_item();
        up(&mutex);
        up(&empty);
        consume_item(item);
    }
}

```

1. `mutex`信号量的作用是保证只有一个进程可以进入临界区
2. `empty`和`full`信号量的作用是实现同步，保证当缓冲区满的时候生产者停止运行，以及当缓冲区空的时候消费者停止运行。

### 2.3.6 互斥量

如果不需要信号量的计数能力，可以使用信号量的简化版本——**互斥量**。互斥量仅适用于管理共享资源或一小段代码。

互斥量是一个可以处于两态之一的变量：解锁和加锁。当一个进程/线程需要访问临界区，调用`mutex_lock`，如果该互斥量当前是解锁的，即临界区可用，则调用成功。另一方面，如果该互斥量已经被加锁，调用线程被阻塞，直到临界区中的线程完成并调用`mutex_unlock`。

`mutex_lock`和TSL指令中的`enter_region`很像，但是不一样在于：enter_region进入临界区失败后，会进入忙等待，在内核态中，这个进程会由于时间片用完，调度其他进程运行，可以让拥有锁的进程运行并释放锁。
而如果在用户态中，进程不会释放CPU资源，因此有锁的进程无法运行也就无法释放锁，没有锁的进程又一直在忙等待，不会退出。

所以`mutex_lock`避免了忙等待，在该线程/进程下次运行时，再一次对锁进行测试。

#### 2.3.6.1 futex

futex称为快速用户空间互斥，实现了基本的锁。首先我们知道自旋锁的如果等待时间长，会浪费CPU周期，而如果改用阻塞进程，却有很多花销在内核切换上。因此有人提出了futex的解决方案。

futex能够更快的原因在于，一般而言它实现了锁，但是不需要陷入内核，除非真的有必要。

futex包含两个部分：**内核服务**和**用户库**。内核服务提供了一个等待队列，允许多个进程在一个锁上等待。而等待队列中的进程是需要系统调用才能放入和接触阻塞，因此内核开销会比较大，一般而言能不用就不用。当没有竞争的时候，futex完全在用户空间工作。

简要来说，就是一个线程检查锁是否被占用，如果被占用，会通过系统调用把线程放入等待队列中，直到锁被释放；如果锁没有被占用，则不需要在内核态切换线程，直接调用即可。

#### 2.3.6.2 pthread中的互斥量

pthread提供了一些用来同步线程的函数，本质都是使用一个可以被锁定和解锁的互斥量来保护每个临界区。

互斥量相关的pthread调用函数如下：
- `pthread_mutex_init`：创建一个互斥量
- `pthread_mutex_destroy`：撤销一个互斥量
- `pthread_mutex_lock`：获得一个锁，如果失败会阻塞调用者
- `pthread_mutex_trylock`：获得一个锁，如果失败会返回错误代码
- `pthread_mutex_unlock`：释放一个锁


除了互斥量之外，pthread还提供了另一种同步机制：**条件变量**。条件变量是用在允许线程由于一些未达到的条件而阻塞。

条件变量相关的pthread调用函数：
- `pthread_cond_init`：创建一个条件变量
- `pthread_cond_destroy`：撤销一个条件变量
- `pthread_cond_wait`：阻塞以等待一个信号
- `pthread_cond_signal`：向另一个线程发信号来唤醒
- `pthread_cond_broadcast`：向多个线程发信号来全部唤醒

通过生产者-消费者问题举例：生产者需要将产品放入缓存中，通过互斥量可以进行原子性检查，不受其他线程干扰。而当缓冲区满了，生产者需要一种方法阻塞自己并在以后被唤醒，这就是条件变量的事情。在*信号量*这一小节中，信号量的第一个作用是互斥，第二个作用是同步，这里条件变量也就是同步的作用。

```c
//图2.3.2,利用线程解决生产者消费者问题

#include <stdio.h>
#include <pthread.h>
#define MAX 100000000
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int buffer = 0;

void *producer(void *ptr){
    int i;
    for(i = 1; i<=MAX; i++){
        pthread_mutex_lock(&the_mutex);
        while(buffer!=0){
            pthread_cond_wait(&condp, &the_mutex);
        }
        buffer = i;
        pthread_cond_signal(&condc);
        pthread_mutex_unlock(&the_mutex);
    }
    pthread_exit(0);
}

void *consumer(void *ptr){
    int i;
    for(i=1; i<=MAX; i++){
        pthread_mutex_lock(&the_mutex);
        while(buffer==0){
            pthread_cond_wait(&condc, &the_mutex);
        }
        buffer = 0;
        pthread_cond_signal(&condp);
        pthread_mutex_unlock(&the_mutex);
    }
    pthread_exit(0);
}

int main(){
    pthread_t pro,con;
    pthread_mutex_init(&the_mutex,0);
    pthread_cond_init(&condc,0);
    pthread_cond_init(&condp,0);
    pthread_create(&con,0,consumer,0);
    pthread_create(&pro,0,producer,0);
    pthread_join(pro,0);
    pthread_join(con,0);
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&the_mutex);
}
```

### 2.3.7 管程

由于信号量的使用必须非常小心，出现一点差错都会导致不可预测和不可再现的错误，因此为了更易于编写正确程序，有人提出了一种高级同步原语，称为**管程**。

一个管程是由过程、变量以及数据结构组成的集合，它们组成了一个特殊的模块或软件包。进程可以在任何需要的时候调用管程中的过程，但是它们不能在管程外声明的过程中直接访问管程内的数据结构。

其实简要来说，管程就是一个程序集合，这个程序中有很多子线程，只不过管程为其提供了更方便的管理，使得管程中的线程不能够同时运行。

并且，管程只是一个语言概念。管程有一个重要特性是：任一时刻管程中只有一个活跃进程。这一特性使管程能够有效地挖成互斥。

管程的互斥是由编译器负责完成，程序员不需要自己安排互斥，也不需要关心编译器是如何实现互斥的，他只需要将所有临界区转换成管程过程即可。

现在管程的互斥问题解决，但是同步阻塞的问题还没有解决。管程引入了条件变量和wait/signal操作，从而阻塞和唤醒进程。

但是管程只是一个编程语言概念，绝大多数语言并不支持，只有JAVA有类似的机制。这里就会产生一个问题，信号量和管程的机制都是用来解决访问公共内存的一个或多个CPU上的互斥问题，但是如果这多个CPU不再同一个电脑上，通过局域网相连接，这种原语就失效了，因为信号量的级别太低了。

### 2.3.8 消息传递

解决上述问题的方法就是**消息传递**。

进程间通信的方法使用两条原语：
- `send(destination, &message)`
- `receive(source, &message)`

send向给定的目标发送一条消息，receive从一个给定的源接收一条消息。如果没有消息可用，则接收者可能会被阻塞直到一条消息到达，或者返回错误码。

1. 消息传递系统的设计要点
   1. 防止发送方消息丢失————使用确认
   2. 防止接收方确认丢失————使用序号
   3. 确保双方身份不冒充————身份认证


2. 用消息传递解决生产者-消费者问题

消息传递分成两种模式：**直接消息传递**和**信箱传递**。如果使用直接消息传递，只需要每个进程知道的消息发向哪和从哪里接受到消息。直接消息传递下，由于没有缓冲机制，该方式假如发送进程先行发送，执行Send()原语，则发送的进程阻塞，直到接受进程执行Receive（）原语进行接受，反之也是一样的。所以没有缓冲，所以发送进程和接受进程之间必须交替的进行。

信箱是一个用来对一定数量的消息进行缓冲的地方。当一个进程试图向一个满的信箱发消息的时候，将会被挂起，直到信箱内有消息被取走，从而为新消息腾出空间。

解决生产者-消费者问题的代码：略。

### 2.3.9 屏障

这个同步机制是准备用于进程组，之前的同步和互斥机制都是用于双进程的生产者-消费者类情形。而有些应用中划分了若干阶段，并且规定除非所有的进程都就绪准备着手下一个阶段，都则任何进程都不能进入下一个阶段。

这就需要在每个阶段的结尾安置**屏障**来实现。当一个进程到达屏障时，会被屏障阻拦，直到所有进程都到达屏障为止。

### 2.3.10 避免锁：读-复制-更新

最快的锁就是没有锁。但是问题在于没有锁的情况下，我们能否堆共享数据结构进行并发的读写访问？通常情况下是不可以的。

考虑这样一个例子，进程A在对一个数组排序，进程B在计算数组的均值，而因为A在数组中将数值来回移动，会导致B可能多次遇到某些数值，就会产生错误。

然而在某些情况下，我们可以允许写操作来更新数据结构，即便还有其他进程正在使用它。诀窍在于确保每一个读操作要么读取旧的数据报本，要么读取新的数据版本，绝不能是一半新一半旧的组合。

读-复制-更新(Read-Copy-Update)RCU，简单来说就是读可以随意，但是更新前需要复制一个副本，等合适的时候进行回调，将旧版本替代。但是旧版本不会立刻消失，而是会等一个宽限期。

### 总结

这一小节讲了非常多的进程间通信机制，需要进行一个总结。

首先我们知道进程会存在**竞争条件**(2.3.1)，因此需要对**临界区**(2.3.2)进行控制来避免竞争条件，就有了以下的尝试和机制：
1. 使得进程等待，不阻塞(2.3.3)：
   
   有很多尝试，但是都算**自旋锁**的思想，即进程一直等待，直到能够进入临界区为止。

   但是自旋锁又会产生优先级反转和浪费CPU资源的问题。
2. 利用睡眠与唤醒(2.3.4)
   
   针对优先级反转和浪费CPU资源的问题，有人提出了使用进程间原语阻塞无法进入临界区的进程，即`sleep`和`wakeup`。

   但是由于判断条件和`sleep`/`wakeup`原语中间不是原子性的，会产生刚读取条件，就切换线程，使得睡眠与唤醒无效，因此用睡眠与唤醒还是不能很好的解决竞争条件问题。
3. 利用信号量(2.3.5)
   
   由于读取-判断-睡眠/唤醒操作不能够分割，因此有人提出了**信号量**这个新的变量类型。对于信号量只有两种操作`down`和`up`，但是这两种操作都是原子操作，也就是说检查数值、修改变量以及可能发生的睡眠操作都是不可分割的。

   同时，信号量可以分成两种作用：互斥和同步。

4. 使用互斥量(2.3.6)
   
   互斥量就是信号量的简化版，互斥量只有两种状态：解锁/加锁。如果加锁成功，那么进程就可以进入临界区；如果加锁不成功，进程就会被阻塞。

5. 使用管程(2.3.7)
   
   由于信号量的使用必须非常小心才能避免错误，所以有人提出了管程这个概念。管程不是一个机制，而是一种高级同步原语的编程语言概念。设计者的思想是将线程互斥的思想利用编译器进行管理，不需要人为的设置信号量，简化了程序的编写。

   但是管程毕竟只是一种语言概念，很多语言并不支持，主流语言中只有Java有类似的机制。除此之外，由于现在很多进程不再是同一个CPU或同一台主机的事情，而在网络中进行进程通信，原语级别就不够了。

6. 使用消息传递(2.3.8)
   
   为了解决网络中的进程通信问题，有人就提出了消息传递机制，利用`send`和`receive`来实现消息的传递，而这里又分成两种：直接消息传递和信箱传递。

   直接消息传递的接收方/发送方就是两个进程，如果其中某一个消息未被接收，那么发送方将会阻塞。

   信箱则是多了一个缓冲区，提高了效率。

7. 屏障(2.3.9)
   
   上述的情况都是两个进程产生竞争，如果涉及到进程组，直到这一组进程都达到条件才能进行下一步的情况，就需要使用屏障。

8. RCU(2.3.10)
   
   最后一种同步机制，在于尝试通过不加锁来避免一些读写情况的冲突，通常情况下不允许，但是有时候，例如对于链表的访问，是可以不加锁的。使用读-复制-更新机制是可行的。