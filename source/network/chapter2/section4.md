## 2.4 DNS(Domain Name System)

DNS是：1. 一个由分层的DNS服务器实现的分布式数据库。2. 一个使得主机能够查询分布式数据库的应用层协议。

DNS协议运行在UDP上，使用53端口。并且DNS协议通常是由其他应用层协议所使用的，除了IP地址转换，DNS还可以提供：主机别名、邮件服务器别名和负载分配几个功能。

#### 2.4.1 DNS工作原理概述

DNS在因特网上实现分布式数据库，大致有三种类型的DNS服务器：根DNS服务器、顶级域DNS服务器和权威DNS服务器。是一个树状结构，除此之外，还有一个本地DNS服务器，本地DNS服务器一般由ISP维护，起着代理的作用。

在请求主机和本地DNS服务器的查询是**递归**的，而本地DNS服务器和其他DNS服务器的查询是**迭代**的。

#### 2.4.2 DNS缓存

有缓存以后，大多数情况下，根DNS服务器是被绕过了。

#### 2.4.3 DNS记录和报文

DNS服务器中存储了**资源记录(Resource Record)**，RR提供了主机名到IP地址的映射。RR包含了以下4个字段：

1. Name
2. Value
3. Type
4. TTL: 是该记录的生存时间，决定了资源记录应该从缓存中删除的时间。

- Type=A，则Name是主机名，Value是主机名对应IP地址的映射。
- Type=NS，则Name是个域，Value是个知道如何获得该域中主机IP地址的权威DNS域名服务器的主机名。
- Type=CNAME，则Value是别名为Name的主机对应的规范主机名。
- Type=MX，则Value是个别名为Name的邮件服务器的规范主机名。

DNS报文只有查询和回答两种报文。并且这两种报文的格式是相同的。在此略。