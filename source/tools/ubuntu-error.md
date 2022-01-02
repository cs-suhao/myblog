# Ubuntu使用汇总

## 问题
### sudo apt-get有锁
出现该问题的原因一般是之前在未完成下载的情况下将terminal close。此时 apt-get进程可能没有结束。结果，如果再次运行apt-get install 命令安装软件时，出现如上错误。即有另一个程序正在占用apt-get install进程，由于它在运行时，会占用软件源更新时的系统锁（简称‘系统更新锁’），此时资源被锁。

解决方案：

1. sudo kill PID

找到之前未结束的`apt-get`进程，将其杀死。
```shell
ps aux
ps -e | grep apt-get

sudo kill PID
```

2. 强制解锁

```shell

sudo rm /var/cache/apt/archives/lock  
sudo rm /var/lib/dpkg/lock
```


### 如何安装pip
用系统自带包管理器安装，命令如下：
```shell
sudo apt-get install python3-pip

pip3 --version

//这一句有问题
python3 -m pip install --upgrade pip
```

在前两句进行安装`pip3`以后，对于pip进行更新出现了错误，有两个版本的pip同时出现了。[官方链接给出了回答](https://github.com/pypa/pip/issues/5599)。

官方指出原因在于系统包管理器安装的pip归发行版所有，而如果进行`pip install -U pip`则会更新另一个版本，并且官方提醒不能将`sudo` 和 `pip`一起使用。

#### 推荐
在pip的官方文档中给出了安装方法：
1. 下载[`get-pip.py`](https://bootstrap.pypa.io/get-pip.py)文件
2. 进入文件夹，运行`python3 get-pip.py`

之后使用`pip`安装包的时候，一定要使用如下命令：
```shell
python3 -m pip install *
python3 -m pip --version
```


### 如何修改ubuntu上的时间
在虚拟机上安装ubuntu以后，时间和当前时区的时间不一致，原因是安装的时候默认设置时区为`America/Los_Angeles`，通过以下命令进行调整：
```shell
cat /etc/timezone

timedatectl

sudo timedatectl set-timezone Asia/Shanghai
```