# Chapter 2 线程管理

## 2.1 线程的基本操作

### 2.1.1 启动线程

线程在`std::thread`对象创建时启动，通常使用的是无参数返回的函数。这种函数在执行完毕，线程也就结束了。而在一些情况下，任务函数对象需要通过某种通讯机制进行参数的传递，或者执行一系列独立操作，通过通讯机制传递信号让线程停止。

简单来说，使用C++线程库启动线程，就是构造`std::thread`对象，需要包含头文件`<thread>`，`std::thread`可以用可调用类型构造，将带有函数调用符类型的实例传入类中，替换默认的构造函数。有以下几个方法:

- 方法一：传入函数对象

```cpp
class background_task
{
public:
  void operator()() const // 1
  {
    do_something();
    do_something_else();
  }
};
background_task f;
std::thread my_thread(f);
```
在这里，函数对象被复制到`std::thread`对象内部，函数对象的执行和调用都在线程的内存空间中进行。
注释：
1. `void operator()() const`中前一个`()`是调用符号，后一个是传参列表，这里是空。

需要注意的是，当把函数对象传入到线程构造函数中，需要避免”语法解析“。

- 方法二：传入lambda表达式
  
```cpp

std::thread my_thread([]{
  do_something();
  do_something_else();
});

```

- 方法三：传入函数指针和参数

```cpp
void f(int i);
std::thread(f, 3);
```

- 方法四：使用多组括号或新统一的初始化语法

```cpp

std::thread my_thread((background_task()));

std::thread my_thread{background_task()};
```


如果传递了一个临时变量，而不是命名变量，C++编译器会将其解析为函数声明，而非类型对象。例子如下：

```cpp
std::thread my_thread(background_task());
```
这句相当于声明了一个`my_thread`的函数，该函数带有一个参数（函数指针指向没有参数并返回background_task对象的函数），返回一个`std::thread`对象的函数，而非启动一个线程。

当启动线程后，需要明确等待线程结束还是自主运行，这两个分别对应着：`join()`和`detach()`。在`std::thread`对象销毁前没有做出决定，程序就会终止，也就是`std::thread`的析构函数会调用`std::terminate()`。

如果不等待线程，就必须保证线程结束前，可访问的数据具有有效性。处理这种情况的常规方法：使线程函数的功能齐全，将数据复制到线程中，而非复制到共享数据中。如果使用一个可调用的对象作为线程函数，这个对象就会复制到线程中，而后原始对象就会立即销毁。


### 2.1.2 等待线程完成

如果需要等待线程，线管的`std::thread`需要使用`join()`，就可以确保局部变量在线程完成后才被销毁。

`join()`只是简单粗暴的等待线程完成或不等待，同时调用了`join()`以后，会清理线程相关的存储部分，这样`std::thread`对象将不再与已经完成的线程有任何关联，也就是说一个线程一旦使用过`join()`，之后就无法再次使用`join()`，通过`joinable()`可以得到返回值false。


### 2.1.3 特殊情况下的等待

在`std::thread`对象还未被销魂的时候需要使用`join()`或`detach()`，如果想要分离线程，在线程启动以后就可以直接detach()进行分离。而如果打算等待对应线程，则需要注意调用join()的位置。因为当线程运行之后产生异常，在join()调用之前异常被抛出，则意味着这次调用会被跳过。

通常倾向于在无异常的情况下使用join()时，需要在异常处理的过程中调用join()，从而避免生命周期的问题。

- 方法一：使用`try/catch`块

```cpp
struct func
{
  int& i;
  func(int& i_) : i(i_) {}
  void operator() ()
  {
    for (unsigned j=0 ; j<1000000 ; ++j)
    {
      do_something(i);           // 1. 潜在访问隐患：悬空引用
    }
  }
};

void oops()
{
  int some_local_state=0;
  func my_func(some_local_state);
  std::thread my_thread(my_func);
  my_thread.detach();          // 2. 不等待线程结束
}                              // 3. 新线程可能还在运行

void f()
{
  int some_local_state=0;
  func my_func(some_local_state);
  std::thread t(my_func);
  try
  {
    do_something_in_current_thread();
  }
  catch(...)
  {
    t.join();  // 4
    throw;
  }
  t.join();  // 5
}

```

在上述代码中，`opps()`函数使用detach()分离线程，但是可能存在新线程还在运行而原始线程已经结束的情况，同时新线程中引用了局部变量`i`，会出现引用悬空的情况。

而在`f()`函数中使用`try/catch`块确保了访问本地状态的线程退出后，函数才结束，而函数正常退出会执行5，如果运行过程中抛出异常，则会执行到4。

- 方法二：使用资源获取即初始化方式（RALL, Resource Acquisition is initialization）

这种方式提供一个类，在析构函数中使用join()，使用样例如下：

```cpp
class thread_guard
{
  std::thread& t;
public:
  // explicit避免隐式赋值
  explicit thread_guard(std::thread& t_):
    t(t_)
  {}
  ~thread_guard()
  {
    if(t.joinable()) // 1
    {
      t.join();      // 2
    }
  }
  thread_guard(thread_guard const&)=delete;   // 3
  thread_guard& operator=(thread_guard const&)=delete;
};
struct func;
void f()
{
  int some_local_state=0;
  func my_func(some_local_state);
  std::thread t(my_func);
  thread_guard g(t);
  do_something_in_current_thread();
}    // 4

```

程序执行到4时，由于退出函数，因此局部对象需要被销毁，而局部对象都是压栈的，因此会被逆序销毁。因此thread_guard对象g是第一个被销毁的，而此时线程在析构函数中被加入到原始线程中，也就是步骤2。这样做的好处是，即使do_something_in_current_thread抛出异常，这个销毁依然会发生，线程依然会被加入到原始线程中。

由于线程对象只能加入一次，因此在调用join()之前，需要使用joinable()进行判断。

这里3的语句意思是禁用拷贝构造和拷贝赋值函数，使用`=delete`关键词。

### 2.1.4 后台运行线程

使用detach()会让线程再后台运行，主线程不会等待该线程结束，也无法使用`std::thread`对象引用。如果调用detach()分离线程，之后相应的`std::thread`对象就和实际执行的线程无关，并且该线程也无法加入。
