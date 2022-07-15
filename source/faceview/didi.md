# 滴滴暑期实习面试（女朋友）

## 一面 6.17

1. 自我介绍

### 计算机网络
1. TCP和UDP的区别
2. TCP有哪些机制保证可靠
3. TCP的拥塞控制有了解吗？
4. TCP的三次握手过程
5. TCP和UDP的应用场景

### 数据库
1. 事务有了解吗？
    - 事务的四个特性有了解吗？
2. 数据库的索引有了解吗？
3. redis有了解吗？

### 操作系统
1. 进程和线程的区别
2. 什么时候用进程，什么时候用线程？
3. 比如我用浏览器打开网页，用的是进程还是线程？

### C++
1. C++中指向常量的常量指针和指向非常量的指针？
2. C++中的值传递和引用传递
3. 如果参数是指针，是值传递还是引用传递？

### 算法
1. 简单说一下快排的原理
2. 快排的时间复杂度
3. 手撕代码-两数之和变种
输入一串数字，再输入一个要求的值，输出两数之和等于它的索引值，如果没有就输出(-1,-1)。


## 二面 6.22

1. 自我介绍
2. 有参加过实习或做过一些什么项目吗？
3. 你投递的岗位是后端开发，请说一下你在这个岗位有什么优势？
4. C++自学过程？

### 操作系统
1. 线程在什么情况下会进入死锁呢？

### 计算机网络
1. 如果没有第三次握手，会产生什么问题？如果没有第三次握手，还能传递消息吗？
2. 说一下TCP和UDP的区别

### 计算机组成原理
1. 虚拟内存中页和块有了解吗？

### C++
1. 假设有两个类，B在类A中编译，声明类A的对象，类A和类B的析构函数的执行顺序。
2. 用new实例化对象，回收时会使用析构函数。现在有父类和子类的构造和析构函数，顺序应该是什么？说出理由。

### 算法
1. k个一组反转链表
```c++

struct Node{
    Node *next;
    int val;
    //列表初始化，{}里面什么都不用放
    Node(int val): val(val), next(nullptr) {}
};

class Solution {
public:
    // 翻转一个子链表，并且返回新的头与尾
    pair<ListNode*, ListNode*> myReverse(ListNode* head, ListNode* tail) {
        ListNode* prev = tail->next;
        ListNode* p = head;
        while (prev != tail) {
            ListNode* nex = p->next;
            p->next = prev;
            prev = p;
            p = nex;
        }
        return {tail, head};
    }

    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode* hair = new ListNode(0);
        hair->next = head;
        ListNode* pre = hair;

        while (head) {
            ListNode* tail = pre;
            // 查看剩余部分长度是否大于等于 k
            for (int i = 0; i < k; ++i) {
                tail = tail->next;
                if (!tail) {
                    return hair->next;
                }
            }
            ListNode* nex = tail->next;
            pair<ListNode*, ListNode*> result = myReverse(head, tail);
            head = result.first;
            tail = result.second;
            // 把子链表重新接回原链表
            pre->next = head;
            tail->next = nex;
            pre = tail;
            head = tail->next;
        }

        return hair->next;
    }
};
```