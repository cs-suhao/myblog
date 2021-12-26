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

std::thread my

```

- 方法三：传入函数指针和参数


如果传递了一个临时变量，而不是命名变量，C++编译器会将其解析为函数声明，而非类型对象。例子如下：

```cpp
std::thread my_thread(background_task());
```
这句相当于声明了一个`my_thread`的函数，该函数带有一个参数（函数指针指向没有参数并）
