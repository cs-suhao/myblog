# Chapter 3 线程间共享数据

## 3.1 共享数据带来的问题

当一个或多个线程需要修改共享数据的情况下，就会产生许多问题。可以用双链表的更新过程来说明这个共享数据的问题。

双链表中每一个节点都有一个指针指向下一个节点（后指针），一个指针指向前一个节点（前指针）。当我们从列表中删除一个节点的时候，其前后节点的指针就需要更新，更新的步骤如下：
1. 找到要删除的节点N
2. 更新前一个节点指向N的指针，让这个指针指向N的下一个节点
3. 更新后一个节点指向N的指针，让这个指针指向N的前一个节点
4. 删除节点N

![双链表更新节点过程](image/chapter3/3-1.png)

在过程2和3的时候，整个列表的顺序是错乱的，属于中间未稳定的状态。在这时候，如果有线程对该列表进行访问，就会产生非常严重的后果，某一个线程可能刚好访问到要删除的节点N，而其他线程如果按照顺序访问，就会跳过节点N，如果这时候某一个线程再次尝试删除相邻节点，那么这个双链表就断开了，对于整个数据结构产生永久性的损坏。

### 3.1.1 条件竞争

并发中竞争条件的形成，取决于一个以上线程的相对执行顺序，如果对于系统的不变量不会造成破坏，这就是一种良性竞争，因为谁先谁后并不会产生严重后果。而如果造成不变量破坏，才会产生竞争条件，也就是双向链表的例子，这里就是我们需要避免的恶性竞争条件。

### 3.1.2 避免恶性竞争条件

解决恶性竞争条件，有几种选择：
- 对数据结构采用某种保护机制
- 对于数据结构和不变量的设计进行修改，保证每一个不变量保持稳定的状态，也就是所谓的无锁编程
- 使用事务的方式处理，参照GitHub提交代码的思想，在push之前如果有改变必须先pull才能提交


## 3.2 互斥量

和操作系统中互斥量的思想一致，在访问共享数据前，将数据锁住，在访问结束，再将数据解锁。其他线程必须等待前线程解锁后才能进行访问。但是和操作系统中提出的问题一致，该方法可能会造成死锁。

**死锁**的概念就是，线程A和线程B需要同时拥有两个资源a和b才能进行下一步，然后A锁住a，B锁住b，两个线程都在死等对方的资源，又不会释放拥有的资源，而导致循环等待。

### 3.2.1 C++中的互斥量

C++中通过`std::mutex`创建互斥量实例，可以通过lock()和unlock()上锁和解锁。但是实践中，C++标准库提供了一个RAll语法的模板类`std::lock_guard`，在构造时能提供已上锁的互斥量，在析构的时候进行解锁，从而保证一个已锁互斥量能够被正确解锁。

```cpp
#include <list>
#include <mutex>
#include <algorithm>
#include <thread>
#include <iostream>

std::list<int> some_list = {1, 2, 3, 4, 5}; // 1
std::mutex some_mutex; // 2

void add_to_list(int new_value)
{
    std::lock_guard<std::mutex> guard(some_mutex); // 3
    some_list.push_back(new_value);
}

void list_contains(int value_to_find, bool& result)
{
    std::lock_guard<std::mutex> guard(some_mutex);
    if (std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end())
    {
        result = false;
    }
}

int main()
{
    bool result = true;
    std::thread t2(list_contains, 6, std::ref(result));
    std::thread t1(add_to_list, 6);
    std::cout << result << std::endl;
    return 0;
}
```

这里产生了几个问题：
1. std::mutex是如何绑定共享数据的？
   答：在lock_guard函数后，只要执行了lock_guard，对于后面的操作和数据都是互斥的。
2. std::thread线程在运行函数时，如果函数有返回值，是如何返回的？
   ？？

注释：
1. 全局变量
2. 全局互斥量
3. 调用lock_guard保护全局变量

在大多数情况下，互斥量通常会与需要保护的数据放在同一类中，而不是定义成全局变量。同时我们能发现，具有访问能力的指针和引用虽然本身会被互斥量限制，但是其指向的数据并不会被限制。

### 3.2.2 保护共享数据时的问题

有时候，如果成员函数通过返回值或输出参数的形式，向调用者返回受保护的数据的指针或引用，数据就不会被保护，下面就是一个例子：
```cpp
class some_data
{
  int a;
  std::string b;
public:
  void do_something();
};
class data_wrapper
{
private:
  some_data data;
  std::mutex m;
public:
  template<typename Function>
  void process_data(Function func)
  {
    std::lock_guard<std::mutex> l(m);
    func(data);    // 1 传递“保护”数据给用户函数
  }
};
some_data* unprotected;
void malicious_function(some_data& protected_data)
{
  unprotected=&protected_data;
}
data_wrapper x;
void foo()
{
  x.process_data(malicious_function);    // 2 传递一个恶意函数
  unprotected->do_something();    // 3 在无保护的情况下访问保护数据
}
```
注释：
1. 将保护的数据传递给用户函数
2. 传递恶意函数，为何是恶意函数，因为我们这里使用了some_data的引用
3. 由于使用的是保护数据的引用，所以我们可以绕开互斥量操作，对保护的数据进行操作。

### 3.2.3 定位接口间的条件竞争

即使使用了互斥量来保护共享数据，但是由于接口的问题，仍然会遇到条件竞争的问题，例如构建`std::stack`结构的栈，有以下五个操作：
- push()一个新元素入栈
- pop()一个元素出栈
- top()查看栈顶元素
- empty()判断栈是否是空栈
- size()了解栈中有多少元素


考虑这样一个过程：如果栈是非共享的，我们可以使用empty()检查栈是否非空，然后用top()访问栈顶元素。然而在多线程中，对于共享的栈对象，这样的调用就不安全了，因为当一个线程使用empty()后，另一个线程可能使用pop()删除了最后一个栈元素，而导致其使用top()错误。所以即使我们能够使用互斥量对栈进行保护，依旧不能组织条件竞争。

还有一些问题：假设有一个`stack<vector<int>>`，vector是一个动态容器，当我们拷贝一个vector，标准库会从堆上分配很多内存来完成这次拷贝，而当系统处于严重资源限制的情况下，这种内存分配就会失败，所以vector的拷贝构造函数可能会抛出异常。当我们用pop()函数返回“弹出值”， ，这个值被返回到调用函数的时候，该元素在栈中移出了，但是拷贝又因为资源限制失败了，所以这个值就完全消失了。

对于这个问题，有一些选择的方法可以解决：

1. **传入一个引用**
   这个方法是将变量的引用作为参数，传入pop()函数中获取想要的“弹出值”
   ```cpp
   std::vector
2. **无异常抛出的拷贝构造函数或移动构造函数**
3. **返回指向弹出值的指针**
4. **1+2 或 1+3**


