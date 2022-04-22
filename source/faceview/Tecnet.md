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