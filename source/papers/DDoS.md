# DDoS论文

- [x] Mitigating DNS random subdomain DDoS attacks by distinct heavy hitters sketches

  这篇文章针对随机子域名DDoS攻击提出了distinct Heavy Hitter检测的方法，由于随机子域名DDoS攻击会在短时间内，针对目标域名产生大量随机的、无意义的子域名，这也会导致某一个域名的子域名统计数量会在短时间内异常增长。因此本文提出基于概率采样和分层大流的原理，在未受到攻击的时候统计正常情况下各个域名查询的基线值，之后在攻击检测中使用基线值作为DDoS攻击的告警阈值。在攻击检测的过程中，通过哈希函数计算采样概率，并将通过采样的域名对存入固定长度的缓存中，之后统计其域名和子域名的数量，当其值超出阈值会把该域名对作为攻击流量生成的子域名作为黑名单上报，从而实现随机子域名DDoS的缓解措施。

- [x] SkyShield: A sketch-based defense system against application layer DDoS attacks

  本文针对应用层泛洪型DDoS攻击，使用了一种基于sketch和bloom filter的防御方案，该方案基于两种预设：1.DDoS攻击的流量速率相对于正常流量的速率更快。2.正常流量在sketch中的流量分布是稳定的。
  在上述两个预设条件下，在检测攻击阶段，作者利用两个sketch之间的改进海灵格距离作为异常流量的判断依据，找出异常流量所在sketch中的相应位置，并为了减少追溯源IP所带来的计算开销，在缓解攻击阶段使用了bloom filter作为黑名单、白名单来过滤已知的攻击流量和正常流量。

- [x] NETHCF: Enabling line-rate and adaptive spoofed IP traffic filtering

  本文作者为解决IP地址欺骗问题，在可编程交换机中实现了NETHCF，一种基于传输路径跳数统计的过滤方法。该方法部署在中间网络的可编程交换机中，采用解耦合的思想，在数据面实现NETHCF的cache部分，在控制面实现NETCHF的mirror部分，利用了可编程交换机线速处理和全局流量性质，解决了之前在主机端侧或中间服务器部署带来的带宽浪费、处理速度不足、网络动态性适应慢等痛点。作者在Barefoot Tofino交换机中部署NETHCF，通过实验证明其有效性。

- [x] In-Network Volumetric DDoS Victim Identification Using Programmable Commodity Switches

  本文作者针对流量型DDoS攻击设计了一种基于可编程交换机的受害者识别方法，作者基于Bitmap和Count-Min Sketch的思想，结合两者设计了BACON的sketch数据结构，能够在可编程交换机中统计二元组的同时，统计不同目的地址的数目，从而确定受害者的IP地址，之后上报给控制器，由控制器进行限速或丢包操作。并且，作者还在Tofino交换机中用P4语言实现了该方法，能够在线速条件下快速准确识别DDoS受害者。

- [x] Tracking Normalized Network Traffic Entropy to Detect DDoS Attacks in P4

  本文作者专注于实现P4可编程交换机进行DDoS攻击检测，由于P4语言生态暂未完善，因此无法支持很多复杂计算，例如for循环，导致很多稍微复杂一些的检测算法无法实现。作者针对这一问题，设计了P4LogLog、P4Entropy和P4DDoS三种P4功能函数，能够仅在可编程交换机的数据面就计算出流基数、流熵和DDoS攻击，而无需控制面过多的参与。

  本文和UnivMon等文章的不同在于，之前的文章中数据面仅仅用于支持统计计数过程，而更复杂的流基数估算、流熵值计算等都需要控制面抓取数据面的数据进一步进行计算，而本文可以直接在数据面计算出需要的基数、熵值等。

- [ ] Collaborative detection and mitigation of DDoS in software-defined networks

- [ ] Identifying Application-Layer DDoS Attacks Based on Request Rhythm Matrices

- [ ] A fast all-packets-based DDoS attack detection approach based on network graph and graph kernel

- [ ] Real time DDoS detection using fuzzy estimators

- [ ] Offloading real-time DDoS attack detection to programmable data planes

- [ ] Poseidon: Mitigating Volumetric DDoS Attacks with Programmable Switches

- [ ] JESS: Joint Entropy-Based DDoS Defense Scheme in SDN

- [ ] Euclid: A Fully In-Network, P4-based Approach for Real-Time DDoS Attack Detection and Mitigation





## HHH问题

- [ ] Efficient computation of frequent and top-k elements in data streams.

- [ ] Hierarchical heavy hitters with the space saving algorithm

  HHH问题可以扩展为用分层的方式管理频繁项数据，本文建立在space saving算法基础上，也就是文献【Efficient computation of frequent and top-k elements in data streams.】，在space saving算法中，维持计数器，并只对集合中的部分子集元素维护估算值，从而统计频繁项和top-k。

  > space saving的具体步骤：首先维持计数器，当一个项到达，首先检查是否在计数器中，如果在，则相应计数器值相加；若不在，则替换计数器中值最小的项，并将计数器值相加以后替换。虽然这种计数方式有悖直觉，但是实验表明，只要计数器T的尺寸足够大，那么所有的HH都会出现在最终的T中。

  本文的算法为：在每一层上保持一个Heavy Hitter算法的实例，对于每个更新e，我们计算e的所有一般化，并将每个e分别插入到不同的Heavy Hitter数据结构中。当确定哪些前缀输出为近似的HHHs时，我们从晶格的底部开始，并向顶部工作，使用包含-排除原则来获得每个前缀的条件计数的估计。我们输出任何估计条件计数超过阈值φN的前缀。

- [ ] Constant time updates in hierarchical heavy hitters

  本文的算法在文献【Hierarchical heavy hitters with the space saving algorithm】的基础上进一步改进，原先需要在HHH的每一个晶体格中维护HH实例，并在新元素到达的时候，对于每一个前缀晶体格单元进行更新。这样一来每一个元素的插入复杂度会随着维度和分层大小而上升。本文在插入更新的操作上进行更新，不再是插入更新每一层，而是通过概率，只更新某一层的HHH，并更新其对应的HH实例，从而让插入更新操作的时间复杂度降低成Ｏ（１），也就是常数时间。

  与此相对应的，本文还对于查询操作的准确度进行了松弛化定义，证明了在一定程度上，准确度和精度不会下降的很厉害，并且对于插入时间的消耗来说，是可以接收的。

  <img src=".\image\Constant update time in HHH-1.PNG" alt="Constant update time in HHH-1" style="zoom:60%;" />

- [ ] Volumetric Hierarchical Heavy Hitters

  本文是在【Constant time updates in hierarchical heavy hitters】基础上进行改进的，作者注意到新的问题，在流量型HHH检测中，之前仅针对数据包进行采样计数的方法，有着高达50％的误差，因此对于数据包进行采样是不可行的。因此作者针对该问题，对字节数进行采样，随着数据包字节数大小改变采样的概率。也就是说，字节数大的数据包会被重复采样，而字节数小的数据包可能会被忽略。

  <img src=".\image\VRHHH-2.PNG" alt="VRHHH-2" style="zoom:60%;" />

  <img src=".\image\VRHHH-1.PNG" alt="VRHHH-1" style="zoom:60%;" />

- [ ] Finding hierarchical heavy hitters in streaming data

- [ ] Revealing Hidden Hierarchical Heavy Hitters in network traffic

- [ ] Finding Hierarchical Heavy Hitters with the Count Min Sketch

- [ ] Recursive latice search: Hierarchical heavy hiters revisited









