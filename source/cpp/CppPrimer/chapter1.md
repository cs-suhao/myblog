# Chapter 1 变量和基本类型
## 1.1 基本内置类型
- **关于内置类型的机器实现与存储：**
  - 可寻址的最小内存块称为“字节”
  - 存储的基本单元称为“字”
  - 字比字节大，一般字节由8bit组成，而字一般是32/64bit。

- **关于类型转换**
  - 有符号类型和无符号类型数不能放在一起运算，或者说，当两者放在一起进行运算时需要格外注意
  - 无符号类型在循环体中使用需要注意，有可能无限循环

- **字面值常量**
  - 字面值常量首先只能通过其值称呼，并且是常量无法修改

- **变量定义与初始化**
  - 初始化不是赋值，初始化的含义是创建变量时赋予其一个初始值，而赋值的含义是把对象的当前值擦除，用新值替代。
  - 初始化的形式：
    ```c++
    int sold = 0;
    int sold = {0};
    int sold(0);
    int sold{0};
    ```

  - 默认初始化：定义于函数体内部的内置类型变量如果未被初始化是未定义的。类的对象未被初始化，其值由类决定。


- **声明与定义**
  - 声明使得名字为程序所知，一个文件如果想要使用别处定义的名字必须包含对这个名字的声明。
  - 定义负责创建与名字有关联的实体。
  - 声明和定义的区别在于，定义会申请存储空间，并有可能给变量赋初值。

如果想要声明，需要使用关键字`extern`，并且不能对相应的变量赋初始值，如果在函数体内部对extern变量赋予初始值会出错，因为由于函数体内变量作用域仅限于函数体内的原因，将会导致出错。

## 1.2 复合类型

### 1.2.1 引用

引用是为对象起了另一个名字，声明符写成`&d`的形式。
- 引用必须被初始化。
- 定义引用时，程序将引用和其初始值绑定在一起，而不是将初始值拷贝给引用。
- 引用只能绑定在对象上，不能和某个字面值绑定。

### 1.2.2 指针

指针和引用都是对其他对象的间接访问，但指针和引用有几个不同：
1. 指针本身是一个对象，允许对指针赋值和拷贝。
2. 指针无需在定义时赋初值。
3. 不能定义指向引用的指针。


空指针是没有指向任何对象的指针，空指针的生成有以下几种方法：
```c++
int *p1 = nullptr;
int *p2 = 0;
int *p3 = NULL;
```

新标准下，最好使用nullptr，而不是NULL。`nullptr`是一种特殊类型的字面值，其可以转换为其他任意类型的指针类型。但是空指针不是`void*`指针，两者一个是声明符，一个是特殊字面值。

**void* 指针**

void\*是一种特殊类型的指针，可存放任意对象的地址。但是void类型的指针作用有限



