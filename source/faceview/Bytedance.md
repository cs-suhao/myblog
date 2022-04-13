# 字节2022.4.13 一面（女朋友）
## 其他
1. 问了一个愿不愿意学Go语言？
2. 愿不愿意去北京？
## C++
- 指针和引用的区别
- 面向对象的三个基本特征
  - 了解多态吗？有什么作用
- 用过STL吗，介绍一下？
- 了解C++虚指针吗？//
- C++代码到执行的过程

## 操作系统
- 如何理解进程和线程
- 如果有一个4核CPU，I/O复用比较密集，该开多少线程？
- 了解淘汰策略吗？FIFO/LRU/LFU

## 逻辑题
- 有一个5G的大文件，每一行都是一个整数，怎么在磁盘空间无穷，内存只有1G的情况下对其进行排序？
  - 分治再合并，归并排序的思想

## 多线程和网络
- TCP是如何保证可靠通信呢？
- 列举常见的I/O模型？
- 拥塞控制了解吗？（不能和流量控制混淆了）
  - 慢开始
  - 拥塞避免
  - 快恢复
- 多线程产生死锁的原因

- A线程和B线程，B线程等A线程执行完，B才执行，想一个思路？
  - 信号量？Flag?A中写一个判断程序？

## 数据库
- 范式设计？详细讲一下这些范式的区别
- mysql的事务隔离级别？
- mysql的可重复读的底层实现

## Linux命令
- 用root权限加可读的命令
- 如何通过进程名字找到监听端口？

## 算法题

- 最长无重复子串
```cpp
class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        map<char, int> Allstring;
        int start=0;
        int end=0;
        int ans = 0;
        int length = s.size();
        map<char, int>::iterator iter;
        if(length == 0){
            return 0;
        }
        for(int i=0;i<length;i++){
            iter = Allstring.find(s[i]);
            if(iter != Allstring.end()){
                int temp = iter->second;
                if(temp >= start){
                    ans = max(ans,end-start);
                    (*iter).second=i;
                    start = temp+1;
                    end++;
                }else{
                    ans = max(ans, end-start+1);
                    end++;
                    (*iter).second=i;
                }
            }else{
                Allstring.insert(pair<char,int>(s[i],i));
                end++;
                if(i==length-1){
                    ans = max(ans,end-start);
                }
            }
        }
        return ans;
    }
};
```