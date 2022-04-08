# c++的const用法
## 1. const含义

用`const`修饰的变量类型称为常量，常类型的变量或对象的值无法修改。

## 2. const常量
### 2.1 为什么使用const常量
- 增强可读性、减少维护量。
- 提高健壮性：使用`const`修饰的变量无法修改。
- 节省空间，避免不必要的内存分配。

### 2.2 const常量与define常量的区别
- 类型检查不同：
  
  const常量具有数据类型，而define宏定义没有数据类型，编译器无法检查错误：

    ```cpp
    define a 2.0
    const float a 2.0;
    ```
- 存储方式不同：
  
  const常量在内存中只有一份拷贝，分配在常量区(可能是堆，也可能是栈)；而define常量在使用时进行替换，因此有一个地方用到了就有一次拷贝。

## 3. const使用

### 3.1 修饰局部变量

当局部变量需要在别的文件中使用时，必须在初始化的时候加上`extern`修饰。如果不是`const`，则不需要使用`extern`修饰。

```cpp
// file1.cpp
int ext;
extern const int ext_const = 5;

//file2.cpp
#include <iostream>

extern int ext;
extern const int ext_const;
int main(){
    std::cout<<(ext+10)<<std::endl;
    std::cout<<(ext_const)<<std::endl;
}
```

### 3.2 const常量初始化

`const`常量在定义时一定要进行初始化，一旦初始化完成后就无法修改，任何有`=`的意图都会被编译器标记为错误。

并且常量定义的类型只有基本类型，不能定义结构体类型。

### 3.3 指针与const

当指针和`const`结合，只需要看`const`关键字在`*`号的左边还是右边，如果是以下两种情况：
```cpp
const int * a;
int const * a;
```
这两种`const`都在`*`左边，`const`关键字修饰指针所指向的变量，也就是`a`是常量，不能进行修改。

另一种情况如下：
```cpp
int * const a;
const int * const a;
```
当`*`右边出现`const`时，表示`const`修饰的是指针本身，也就是常指针。

#### 3.3.1 指向常量的指针
   
指针本身不是常量，但是指向的值是“常量”。这里加引号是因为，指针指向的对象可以是非const对象，但是都不能通过指针去修改指向对象的值。

  ```cpp
  const int* ptr;
  *ptr = 10;//error 不能通过指针修改
  
  int val = 10;
  //case1: ok
  ptr = &val;
  //case2: ok
  val = 12;

  //case3: ok
  int val2 = 14;
  ptr = & val2;

  //case4: error
  *ptr = 2;
  ```
同时，不能使用void`*`指针保存const对象的地址，必须使用const void`*`。

#### 3.3.2 常指针

const指针必须进行初始化，并且const指针的值不能修改，也就是指向的地址不能改变，但是指向的对象只要不是常量，是可以进行修改的。
```cpp
int num = 0;
int *const p = &num;
//case 1:ok
*p = 10;

//case 2:ok
int *p1 = &num;
*p1 = 20;

```

另外需要注意的是，当把一个const常量的地址赋值给常指针时，是不行的，因为const常量对象的地址只能给指向常量的指针。

```cpp
const int num = 0;
//error
int * const p = &num;

//ok
const int *const p = &num;
```

#### 3.3.3 指向常量的常指针

见上述两种情况.

## 4. const在函数中

1. const修饰函数返回值

  没有什么特别的,和上述的修饰常量和指针的含义一样,因为修饰的是函数返回值,而返回值是一个变量,就直接和const+变量和指针一致.

2. const修饰函数参数

- 传过来的参数和指针本身在函数中不可变
```cpp
void func(const int var); // 传递过来的参数不可变
void func(int *const var); // 指针本身不可变
```
  没有什么特别的,只是作为一个强调.

- 参数所指向的内容为常量不可变
```cpp
void StringCopy(char *dst, const char *src);
```
  函数内部无法修改指针指向的对象.

- 参数为引用,防止改变并提高效率

```cpp
void func(const A &a)
```

这里是为了提高效率和安全性,因为如果写成`void func(A a)`,因为`A`并非内部数据类型,所以函数体内部将产生临时对象,但是临时对象的构造\复制\析构都会消耗时间.

所以可以改称`void func(A &a)`,但是这里引用传参可能会改变`a`,因此最终加上const.

## 5. 类中使用const

注意,const放在函数前是修饰返回值,**放在后面才是修饰成员函数本身**,如下:
```cpp
//修饰返回值
const int func();

//修饰成员函数
int func() const;
```
- const对象只能操作含const成员函数,而非const对象可以操作所有成员函数.
- const成员函数不能操作非const成员函数,会报错.
- 类中的const变量需要通过列表初始化,并且该常量只在该对象生存周期不变,并不是只该类不变.
- const成员函数中不能对数据进行改变,也就是如果检测到`=`会报错.
