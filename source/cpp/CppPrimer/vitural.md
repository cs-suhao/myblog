# 虚函数和抽象类

## 1. 纯虚函数和抽象类

### 1.1 关于纯虚函数
- **虚函数**：在类成员方法的声明语句前加`virtual`关键字，子类可以重写也可以不重写。
- **纯虚函数**：没有函数体的虚函数，并且赋值0。子类必须重写。

### 1.2 关于抽象类

- **抽象类**：包含纯虚函数的类。

抽象类只能作为基类来派生新类使用，不能创建抽象类的对象。但是可以通过抽象类的指针和引用，指向由抽象类派生出来的类的对象，也就是说:
- `A a`：非法
- `A *a = new A()`：非法
- `A *a`：合法

在抽象类的成员函数内可以调用纯虚函数，在构造函数/析构函数内部不能使用纯虚函数。并且，如果一个类从抽象类中派生，那么只有它实现了基类中的所有纯虚函数，才能称为非抽象类。

在抽象类中有以下几个注意点：
- 只要有一个纯虚函数，就会使一个类称为抽象类。
- 抽象类的引用和指针，可以指向由抽象类派生出的类的对象。
- 抽象类可以有构造函数。
- 构造函数不能是虚函数，但是析构函数可以。

## 2. 多态

封装、继承、多态是C++三大特性，其中多态和继承是有区别的，多态指的是