# Chapter 2 字符串、向量和数组

## 2.1 标准库类型vector

标准库类型vector表示对象的集合，其中所有对象的类型都相同。同时，vector是模板而非类型，需要制定所存放对象的类型才能实例化成什么样的类。

### 2.1.1 定义和初始化vector对象

初始化vector对象的方法：
```c++
vector<T> v1;        //默认初始化
vector<T> v2(v1);    //v2拷贝v1中所有元素进行初始化
vector<T> v2 = v1;   //
vector<T> v3(n, val);
vector<T> v4(n);
vector<T> v5{a,b,c...};
vector<T> v5={a,b,c...};
```

**列表初始化**一般都是使用花括号的情况下，但是如果花括号中无法处理成列表初始化时彩绘考虑其他初始化方式。例如以下方式：
```c++
vector<string> v6{10};
vector<string> v7{10, "hi"};
```