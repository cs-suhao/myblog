# 腾讯



## 2022.4.15 腾讯音乐 一面（女朋友）
1. 自我介绍

## 项目
- 介绍一下射频指纹
- 以手机为例，说明一下存在什么问题，解决什么问题
- 讲一下在这个项目中做了什么
- 提取了信号中的哪些特征？用什么方法？
- 学习这些知识，除了上课还有哪些渠道？

## 操作系统
- 操作系统使用2进制，但是为什么内存地址用16进制表示？
- 一个浮点数在内存中如何表示？
- 虚拟内存是什么？
- C++中代码段、数据段这些是什么，放什么？
    - 栈区放的是什么？堆区放什么？
- 不同进程如何共享变量？
- 如果我有一个很大的数组，开多个线程如何并行？
- 信号量如何使用的？修改信号量如何保证其他线程无法使用

## 计网
- 讲一下HTTP的cookie
- 讲一讲代理服务器如何运作：Web缓存服务器！
    - 主机请求HTTP，然后发往代理服务器，代理服务器有的话就返回，没有的话代理服务器就会发往目的HTTP服务器
    - 如果代理服务器的目的HTTP服务器改变了？
- TCP的传输可靠性
    序号、停止等待、校验和（16bit和相加，取反码）、自动重传、滑动窗口（回退N步，选择重传）
- TCP三次握手
    SYN=1 seq=随机1 today is a drunk day
    SYN=1 ACK=随机1+1 seq=随机2
    SYN=0 ACK=随机2+1 
- TCP四次挥手
    FIN=1
    等待 2MSL 没等到就关了

## 算法题
- 开放性，有两个超大数组，数组都是整数，无规律排序，找到相同的数？
    哈希
- 快排手撕
```cpp
//快速排序
void quick_sort(int s[], int l, int r)
{
    if (l < r)
    {
        //Swap(s[l], s[(l + r) / 2]); //将中间的这个数和第一个数交换 参见注1
        int i = l, j = r, x = s[l];
        while (i < j)
        {
            while(i < j && s[j] >= x) // 从右向左找第一个小于x的数
                j--;  
            if(i < j) 
                s[i++] = s[j];
            
            while(i < j && s[i] < x) // 从左向右找第一个大于等于x的数
                i++;  
            if(i < j) 
                s[j--] = s[i];
        }
        s[i] = x;
        quick_sort(s, l, i - 1); // 递归调用 
        quick_sort(s, i + 1, r);
    }
}
```