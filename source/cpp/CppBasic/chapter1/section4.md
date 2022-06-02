## 1.4 类型别名
有时候我们需要为某种类型取一个新名字。例如：

```
using Pchar = char*;
using PF = int(*)(double);
```
类型别名就是类型的另一个名字，早期还有一种语法，使用typedef关键字，例如：
```
typedef int int32_t;
typedef short int16_t;
```
使用using关键字可用于引入一个template别名，例如：
```
template<typename T>
    using Vector = std::vector<T, My_allocator<T>>;
```


