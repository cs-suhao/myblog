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

在使用detach()前，同样需要检查是否能对该`std::thread`对象使用，和join()一样，利用joinable()函数进行检查，如果返回的是true就可以使用。

文字办公软件可以认为是分离线程的具体用例，当WPS同时处理多个文档，虽然每一个文档页面是独立的，但是却运行在同一个应用实例中，一种内部处理方式就是让每一个文档拥有自己的线程，每一个线程运行同样的代码，并隔离不同窗口处理的数据。

下面的代码展示了这样的思路：

```cpp
void edit_document(std::string const& filename)
{
  open_document_and_display_gui(filename);
  while(!done_editing())
  {
    user_command cmd=get_user_input();
    if(cmd.type==open_new_document)
    {
      std::string const new_name=get_filename_from_user();
      std::thread t(edit_document,new_name);  // 1
      t.detach();  // 2
    }
    else
    {
       process_user_input(cmd);
    }
  }
}
```
如果用户选择打开新文档，则会启动新线程1，这里还将参数传递到新线程，然后2进行线程分离。

## 2.2 向线程函数传递参数

在向`std::thread`构造函数传递参数需要注意，默认参数需要拷贝到线程独立内存中，即使参数是引用的形式，也可以在新线程中进行访问。

但是需要注意，新线程的参数拷贝是不会进行类型检查的，因此如果两者类型不匹配，线程的上下文中会完成类型转换。

- 指针变量传参

当指向动态变量的指针作为参数传递时，需要注意该变量可能会销毁。

```cpp
void f(int i,std::string const& s);
void oops(int some_param)
{
  char buffer[1024]; // 1
  sprintf(buffer, "%i",some_param);
  std::thread t(f,3,buffer); // 2
  std::thread t(f,3,std::string(buffer)); // 3
  t.detach();
}
```
在这个例子中，buffer是一个指针变量，指向本地变量，然后传递到新线程中，另外这里提供的是`char *`，而想要的是`std::string`，但是函数可能在转换前就崩溃，这里就会产生错误。
正确的做法如3.

- 引用变量传参

传递引用变量又可能出现另一种情况，我们期望传递原值的引用，但是由于整个对象被复制了，传递的就是拷贝量的引用。

```cpp
void update_data_for_widget(widget_id w,widget_data& data); // 1
void oops_again(widget_id w)
{
  widget_data data;
  std::thread t(update_data_for_widget,w,data); // 2
  std::thread t(update_data_for_widget,w,std::ref(data)); // 4
  display_status();
  t.join();
  process_widget_data(data); // 3
}
```
在上面的例子中，传递给函数的是data变量经过线程拷贝的引用，而不是原值的引用，因此当线程结束以后，该data变量的值并没有被修改，解决方案是使用`std::ref`将参数转成引用的形式。

- 成员函数指针传参

`std::thread`构造函数可以传递一个成员函数指针作为线程函数，并提供合适的对象的指针作为第一个参数。

```cpp
class X
{
  public:
    void do_length_work();
};
X my_x;
std::thread t(&X::do_length_work, &my_x);
```
这段代码中，新线程将my_x.do_length_work()作为线程函数，my_x的地址作为指针对象提供给函数，如果有对应成员函数的参数，直接在构造函数的第三个参数后加。

- 智能指针传参

我们可以通过向`std::thread`对象传递只能**移动**不能**拷贝**的参数来解决，两者的区别就像剪切和复制。

```cpp
void process_big_object(std::unique_ptr<big_object>);
std::unique_ptr<big_object> p(new big_object);
p->prepare_data(42);
std::thread t(process_big_object,std::move(p));
```


### 2.2.1 线程传参可能遇到的问题

- 问题一：传入临时的callable对象，编译器会认为是函数声明
- 问题二：因为传入指针or局部变量的引用，导致thread function可能访问已经销毁的内容
- 问题三：因为传入指针or局部变量的引用，导致thread function入参时因强制类型转换而访问已被销毁的内容
- 问题四：callable对象参数要求引用，但实际传入的时内部拷贝对象的引用

问题二和问题三出现的原因都是因为主线程和新线程的生命周期不一致导致的，梳理以下`std::thread`对象创建以后的经过：
1. 原线程：调用`std::thread`的构造函数or拷贝赋值运算符
2. 原线程：callable对象和参数被拷贝到新创建的`std::thread`内部
3. 新线程：传入参数到callable对象，这里可能会产生强制类型转换
4. 新线程：调用callable对象
5. ……

这里就是在第二步和第三步出现的问题。


## 2.3 转移线程所有权

C++中通过`std::thread`对象管理线程，而`std::thread`对象本身并非运行的线程，而是该对象和相应的线程绑定，拥有其所有权。因此，线程的所有权是可以在不同`std::thread`对象中转移的，就如同智能指针`std::unique_ptr`一样，可以移动但是不能拷贝。下面的例子中创建了两个执行线程，并且在三个`std::thread`实例中转移所有权。

```cpp
void some_function();
void some_other_function();
std::thread t1(some_function);            // 1
std::thread t2=std::move(t1);            // 2
t1=std::thread(some_other_function);    // 3
std::thread t3;                            // 4
t3=std::move(t2);                        // 5
t1=std::move(t3);                        // 6 赋值操作将使程序崩溃
```
注释：
1. 创建线程t1
2. 将some_function线程的所有权转移给t2，此时t1和执行线程无任何关联
3. 临时std::thread对象启动some_other_function线程，并隐式转移给t1
4. 默认构造t3，无任何关联线程
5. 将t2所拥有的some_function线程转移给t3，此时t2无任何关联线程
6. 将t3所有用的some_function线程转移给t1，但是t1此时拥有some_other_function线程，无法转移，出现错误

这里的例子不仅仅说明线程所有权是可以通过`std::move()`转移，同时还说明了线程只能被转移而不能被丢弃，只能等待完成或分离，因为6出错了。

线程支持移动，也就意味着线程的所有权可以在函数外进行转移，函数可以返回线程对象，而也就可以作为参数进行传递：

```cpp
std::thread f()
{
  void some_function();
  return std::thread(some_function);
}
std::thread g()
{
  void some_other_function(int);
  std::thread t(some_other_function,42);
  return t;
}

void f(std::thread t);
void g()
{
  void some_function();
  f(std::thread(some_function));
  std::thread t(some_function);
  f(std::move(t));
}
```

### 2.3.1 用线程所有权改写RAll

thread_guard类原先使用`std::thread`对象的引用，现在由于知道了std::thread对象支持移动，并且拥有其线程的所有权，我们可以改写：

原始RAll：
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

改写后的：

```cpp
class scoped_thread
{
  std::thread t;
public:
  explicit scoped_thread(std::thread t_):                 // 1
    t(std::move(t_))
  {
    if(!t.joinable())                                     // 2
      throw std::logic_error(“No thread”);
  }
  ~scoped_thread()
  {
    t.join();                                            // 3
  }
  scoped_thread(scoped_thread const&)=delete;
  scoped_thread& operator=(scoped_thread const&)=delete;
};
struct func; // 定义在清单2.1中
void f()
{
  int some_local_state;
  scoped_thread t(std::thread(func(some_local_state)));    // 4
  do_something_in_current_thread();
}                                                        // 5
```

这里有几点不同：
1. 对于线程是否可加入的判断放在了构造函数这里
2. 这里的4出现了初始化传参，为什么没有出现问题一（传入临时的callable对象，编译器会认为是函数声明）？可能是因为带参数和不带参数？

### 2.3.2 线程自动化

如果`std::thread`对象的容器是移动敏感的，那么移动操作同样适用于这些容器，可以将`std::thread`放入`std::vector`中，创建一组线程，进行线程自动化管理

```cpp
void do_work(unsigned id);
void f()
{
  std::vector<std::thread> threads;
  for(unsigned i=0; i < 20; ++i)
  {
    threads.push_back(std::thread(do_work,i)); // 产生线程
  } 
  std::for_each(threads.begin(),threads.end(),
                  std::mem_fn(&std::thread::join)); // 对每个线程调用join()
}
```

其中`std::men_fn`是类成员函数指针，可以将类的成员函数转换为一个可调用对象，这里也就是对于每一个线程调用join()函数。可以简单的将其理解为复用的指针。


## 2.4 运行时决定线程数量

`std::thread::hardware_concurrency()`函数返回能并发在一个程序中的线程数量，可以用于设定程序运行的线程数。

下面有一个并行版的`std::accumulate`，将整体工作拆分成小任务交给每个线程做，对于简单的多线程编程可以作为借鉴：

```cpp
#include <numeric>
#include <iostream>
#include <iterator>
#include <thread>
#include <vector>
#include <algorithm>
#include <functional>
#include <time.h>

template <typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T &result)
    {
        result = std::accumulate(first, last, result);
    }
};

template <typename Iterator, typename T>
T paralle_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);

    if (!length) // 1
    {
        return init;
    }

    // 2
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

    unsigned long const hardware_threads = std::thread::hardware_concurrency(); // 3

    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads); // 4

    unsigned long const block_size = length / num_threads; // 5

    // 6
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);

    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size); // 7
        threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i])); //8
        block_start = block_end;
    }

    accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]); // 9

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join)); // 10

    return std::accumulate(results.begin(), results.end(), init);
}

int main()
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int result = 0;
    double start_paralle, end_paralle, start_std, end_std;
    start_paralle = clock();
    result = paralle_accumulate(input.begin(), input.end(), 0);
    end_paralle = clock();
    std::cout << result << " time use: " << end_paralle - start_paralle << std::endl;

    start_std = clock();
    result = std::accumulate(input.begin(), input.end(), 0);
    end_std = clock();
    std::cout << result << " time use: " << end_std - start_std << std::endl;
    return 0;
}
```

这里程序不复杂，同时我在main函数中进行了测试，程序说明如下：
1. 进行长度判定，如果迭代器传入的容器长度是0，就返回初始值。
2. 进行最大线程和最小线程数限制。
3. 获得硬件可运行的最大线程数。
4. 两者比较取最小值。
5. 根据使用的线程数，将容器等长划分。
6. 因为最后需要汇总各线程的结果，因此这里有results和threads的存储容器，同时因为主线程也作为一个线程运算，所以创建的线程数需要-1.
7. 确定迭代器指针首尾。
8. 创建临时`std::thread`对象，并转移所有权给相应的`thread[i]`。
9. 主线程运算。
10. 对于每一个线程调用`std::thread::join`，等待结果返回。

这里我使用1-10的数组进行测试，可以看到在数量很小的情况下，多线程使用的时间比单线程要多。