## 3.3 UDP

UDP除了最基本的分解/复用和少量的差错检测外，没有增加其他功能。网络管理SNMP协议和DNS协议通常使用UDP协议。

### 3.3.1 UDP报文结构

UDP报文段结构如下所示：
```
0          15                31
source port | destination port
length      | check sum
Application data
```
- length：指示UDP报文段中的字节数（首部+数据），以字节为单位。

### 3.3.2 UDP校验和

UDP校验和用于确定当前UDP报文段，从源到目的移动时，其中的比特是否发生变化。

- 发送方：
    
    发送方的UDP对报文段中所有16比特字的和进行反码运算，求和时遇到的溢出会被回卷。而回卷运算就是进行16比特的加法运算时，如果进位到17位，那么要将第17位和后面16位进行加法运算。

    ```
    0110 0110 0110 0000
    0101 0101 0101 0101
    1000 1111 0000 1100
    ```
    进行加法运算，先进性前两个：
    ```
    0110 0110 0110 0000
    0101 0101 0101 0101
    -------------------
    1011 1011 1011 0101
    ```
    再进行加法，进位并回卷：
    ```
      1011 1011 1011 0101
      1000 1111 0000 1100
      -------------------
     10100 1010 1100 0001

      0100 1010 1100 0001
                        1
      -------------------
      0100 1010 1100 0010
    ```
    最后取反码：
    ```
    1011 0101 0011 1101
    ```

- 接收方

    接收方也做这样的操作，并进行差错检验。

其实有很多链路层协议时提供差错检验的，但是UDP首先提供，是因为：
1. 不能保证左右链路都提供差错检验；
2. 并且有可能报文段存储再某台路由器的内存中，引入了比特差错。

但是UDP虽然提供差错校验，但是没有纠错能力。