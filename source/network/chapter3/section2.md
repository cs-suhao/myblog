## 3.2 多路复用和多路分解

- 多路分解：即将传输层报文段中的数据交付到正确的套接字的工作。
- 多路复用：即将不同套接字的数据块封装并传递到网络层的工作。

在传输层中，有源端口号和目的端口号两个字段，这两个字段就是用于识别哪个套接字。

### 3.2.1 无连接的多路复用和多路分解

在客户端，传输层协议自动并透明的分配端口号，而在服务器端则分配一个特定的端口号。两种`python`方式：
```python
clientSocket = socket(AF_INET, SOCK_DGRAM);

clientSocket.bind(('', 19157));
```

一个UDP套接字是由一个二元组全面标识，如果两个UDP报文具有不同的源IP和源端口，只要它们拥有相同的目的IP和目的端口，会被定位到相同的套接字：
```
<destination IP, destination port>
```

### 3.2.2 面向连接的多路复用和多路分解

一个TCP套接字是由一个四元组全面标识，如果两个具有不同源IP或源端口的TCP报文将被定向到两个不同的套接字上，除非这两个TCP报文是初始创建连接的报文。
```
<source IP, source port, destination IP, destination port>
```

大概的过程如下：
1. TCP客户端创建套接字，并向目的主机、目的端口发送TCP连接请求。
2. 服务器端在对应端口号等待连接，接收到请求报文后，服务器进程会创建一个新的套接字，然后将新的套接字和四元组绑定。TCP连接完成。
3. 服务器将会为每一个连接创建新的进程，每一个进程也将会与新的套接字（四元组）绑定。（当然现在高性能服务器会创建具有新套接字的新线程而非进程）。