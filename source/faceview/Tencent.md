# 腾讯

## 2022.4.15 腾讯音乐 一面（女朋友）
1. 自我介绍

### 项目
- 介绍一下射频指纹
- 以手机为例，说明一下存在什么问题，解决什么问题
- 讲一下在这个项目中做了什么
- 提取了信号中的哪些特征？用什么方法？
- 学习这些知识，除了上课还有哪些渠道？

### 操作系统
- 操作系统使用2进制，但是为什么内存地址用16进制表示？
- 一个浮点数在内存中如何表示？
- 虚拟内存是什么？
- C++中代码段、数据段这些是什么，放什么？
    - 栈区放的是什么？堆区放什么？
- 不同进程如何共享变量？
- 如果我有一个很大的数组，开多个线程如何并行？
- 信号量如何使用的？修改信号量如何保证其他线程无法使用

### 计网
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

### 算法题
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

## 2022.4.19 腾讯音乐 二面（女朋友）

1. 自我介绍
2. 过往的项目经验和课程等，和移动端开发的有契合的吗？
3. 你学到的哪些知识，能够直接或者间接对移动端开发有优势。
4. 有用编程语言写过一点小工具或者小项目吗？
5. 有哪些学到的可以用到实习中？
6. 最近学过什么什么课程

### C++

### 操作系统

### 算法题

二叉树的层序遍历
```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> res;
        list<TreeNode*> s;
        if(!root) return res;
        s.push_back(root);//入栈根节点
        while(!s.empty()){
            int sz = s.size();//得到当前层的节点数量
            vector<int> temp;
            for(int i=0;i<sz;i++){//每遍一个节点就插入其左右节点
                temp.push_back(s.front()->val);
                if(s.front()->left) s.push_back(s.front()->left);
                if(s.front()->right) s.push_back(s.front()->right);
                s.pop_front();
            }
            res.push_back(temp);
        }
        return res;
    }

};
```

```cpp
class Solution {
public:
    int strToInt(string str) {
        bool sign = false;
        int idx = str.find_first_not_of(" ");
        if(idx == -1 ) return 0;
        str = str.substr(idx,str.length()-idx);
        if(str.empty()) return 0;
        if(str[0]=='-' || str[0]=='+') {
            if(str[0]=='-') sign = true;
            str = str.substr(1,str.length()-1);
        }      
        
        if(str.empty()) return 0;
        if(str[0]-'0'>9 ||str[0]-'0'<0 ) return 0;

        string ans_str;
        
        int i = 0;
        for(i=0;i<str.length();i++){
            if(str[i]-'0'>9 ||str[i]-'0'<0 ){
                break;
            }
            ans_str += str[i];
        }
        
        idx = ans_str.find_first_not_of('0');
        if(idx!=-1) ans_str = ans_str.substr(idx, ans_str.length()-idx);
       // cout << ans_str <<endl;
        if(ans_str.empty()) return 0;
        int shu_lenl = ans_str.length();
        int ans_abs = 0;
        for(int i=0;i<shu_lenl;i++){
          //  cout<<ans_abs<<' '<<i<<endl;
            if(sign == true){
                if((INT_MAX - ans_abs )/10 < (ans_str[shu_lenl-1-i]-'0')*pow(10,i-1)) 
                    return INT_MIN;
            }else{
                if((INT_MAX -1- ans_abs )/10 < (ans_str[shu_lenl-1-i]-'0')*pow(10,i-1)) 
                    return INT_MAX;
            }         
            ans_abs += (ans_str[shu_lenl-1-i]-'0')*pow(10,i);
        }
        return sign? -ans_abs:ans_abs;
    }
};
```




### 计网
- 移动APP向服务端请求资源，从应用层到物理层都用到哪些协议呢？
- 简要说一下IP协议
- 一般会用什么端口号呢？
- 根据一个IP地址，是如何找到服务器的呢？物理层是怎么找的呢？
- 从我的手机输入一个网址，经过哪些设备到服务器呢？
    - 从手机-基站-核心网交换机-路由器-服务器
- 应用层常见的协议

## 2022.4.29 腾讯测开 一面（女朋友）

1. 自我介绍

### 项目
1. 详细说一下项目的模块、流程、贡献等
2. 项目中涉及编码工作吗？
3. 关于提取特征，特征是已知的还是推出来呢？
4. 项目是如何分工和协作呢？
5. 整套系统是运行在什么平台上呢？
6. 系统有考虑性能和安全上的测试吗？
7. 数据量有大概的统计吗？

### 专业课程
1. 你们的专业课有哪些？硕士和本科
2. 汇编、数据结构、网络这些课呢？

### C++
1. 指针是什么？
2. C++和C最大的区别？
3. C++中delete和free的区别？
4. C++中的虚函数是什么概念呢？
5. C++是不是一个类型安全的
6. +语言呢？不同的指针类型能不能强制转换？
7. 有用到标准模板库吗？
8. vector和数组有什么区别？
9. static变量在什么时候初始化？

### 操作系统
1. Linux操作系统进程管理的机制？或者说有怎么样的特性？
2. 说一下进程的调度算法
3. 子进程和父进程有什么关系
4. 什么情况下会出现僵尸进程
5. 进程之间的通信方式有哪些？

### 数据库
1. 一般用哪些关系型数据库
2. 查某一个表的所有记录？
3. 对Nosql有了解吗？

### 算法课
1. 查找算法，排序算法？
2. 排序算法中时间复杂度比较小的？
3. 快排原理，时间复杂度是怎么样的？

### 计算机网络
1. 介绍一下TCP和UDP的应用场景
2. 如果TCP这么好，为什么我们不直接使用TCP呢？
3. 有没有使用抓包协议看过呢？
4. TCP报文头
5. 网络协议栈分层，为什么要分层？
6. HTTP协议常用的方法有哪些？
7. Rust什么协议有没有了解？

### 编译原理
1. 知道这门课是干嘛的？

### 开源组件
1. 关于k8s等组件有没有了解？
2. github
3. 持续部署这些方面有没有什么了解？


## 2022.5.5 腾讯测开 二面（女朋友）

1. 自我介绍
2. 项目和实习
3. 常用的网络攻击手段和方式？
4. DDoS攻击作为防守方需要怎么防御呢？
5. 进来以后有想过做什么岗位？
6. 除了leetcode还有什么方法提高呢？
7. Go语言有接触过吗？
8. 平时了解互联网动态吗？
9. 后台server服务等都写过吗？
10. 反问：关于测试开发岗位的要求？更看重学习能力和基础知识。不单单只做自动化测试工具，开始写一些一站式deverce平台，持续部署等。

### 项目
1. 项目的研究成果应用到生活中是怎么样的呢？
2. 识别了这些设备，有哪些应用场景呢？
3. 电磁炮项目中，影响因素有哪些呢？
4. 电磁炮项目中，复杂地形的影响是什么？

### 实习
1. 公司实习的具体情况？

### C++
1. 日常C++语言的使用如何？
2. 自认为对于哪门语言最熟悉？
3. Python中浅拷贝和深拷贝的区别
4. C++中内存分配和内存管理的函数
5. malloc申请的空间没有及时释放会导致哪些问题呢？

### 计算机网络
1. TCP和UDP的区别？
2. 选择使用TCP和UDP使用场景？
3. 描述从浏览器输入网址到打开网页的过程
4. 有没有写过Web Server的服务？

### 算法
1. 链表反转变形，从尾节点开始数
