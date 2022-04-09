# c++的static用法

## 1. 静态变量

### 1.1 函数中的静态变量
函数中的静态变量的特点在于作用域不变，但是在其生命周期中，只会被初始化一次。前一次调用的变量值会在下一次调用时传递。并且变量的存储区为静态变量存储区，一般变量的存储区为栈。

例子如下：
```cpp
#include <iostream> 
#include <string> 
using namespace std; 

void demo() 
{ 
	// static variable 
	static int count = 0; 
	cout << count << " "; 
	
	// value is updated and 
	// will be carried to next 
	// function calls 
	count++; 
} 

int main() 
{ 
	for (int i=0; i<5; i++)	 
		demo(); 
	return 0; 
} 
```
输出结果是：
```shell
0 1 2 3 4
```
如果不加`static`则是`0 0 0 0 0`。

**全局变量和静态全局变量的区别？**
> 作用域不同：
> 
> 1）全局变量是不显式用static修饰的全局变量，但全局变量默认是动态的，作用域是整个工程，在一个文件内定义的全局变量，在另一个文件中，通过extern 全局变量名的声明，就可以使用全局变量。　
> 
> 2）全局静态变量是显式用static修饰的全局变量，作用域是声明此变量所在的文件，其他的文件即使用extern声明也不能使用。

### 1.2 类中的静态变量

因为静态变量只会被初始化一次，因此类中的静态变量为所有对象共享，也就是说，类中的静态变量不能被构造函数初始化。
如果要初始化类中的静态变量，需要在类外使用类名和范围解析运算符初始化。

```cpp

#include<iostream> 
using namespace std; 

class Apple 
{ 
public: 
	static int i; 
	
	Apple() 
	{ 
		// Do nothing 
	}; 
}; 

int Apple::i = 1; 

int main() 
{ 
	Apple obj; 
	// prints value of i 
	cout << obj.i; 
} 
```

如果我们把这句好放在`main`函数中，会报错。

此外，`i`的值是可以修改的，但是就算创建了两个对象，`i`的值会是共享的。

### 1.3 类对象为静态

类对象为静态时，和静态变量相似，作用域不便，但是生命周期是整个程序，如例子：
```cpp
#include<iostream> 
using namespace std; 

class Apple 
{ 
	int i; 
	public: 
		Apple() 
		{ 
			i = 0; 
			cout << "Inside Constructor\n"; 
		} 
		~Apple() 
		{ 
			cout << "Inside Destructor\n"; 
		} 
}; 

int main() 
{ 
	int x = 0; 
	if (x==0) 
	{ 
        // case 1
        static Apple obj_static;
		// case 2
        Apple obj; 
	} 
	cout << "End of main\n"; 
} 
```

输出的内容是：
```shell
//obj_static
Inside Constructor
//obj
Inside Constructor
Inside Destructor
End of main
//obj_static
Inside Destructor
```

### 1.4 类中的静态函数
