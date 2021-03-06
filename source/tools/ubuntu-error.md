# Ubuntu使用汇总

## sudo apt-get有锁
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


## 如何安装pip
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


## 如何修改ubuntu上的时间
在虚拟机上安装ubuntu以后，时间和当前时区的时间不一致，原因是安装的时候默认设置时区为`America/Los_Angeles`，通过以下命令进行调整：
```shell
cat /etc/timezone

timedatectl

sudo timedatectl set-timezone Asia/Shanghai
```


## 如何设置Ubuntu上GitHub的代理
在Ubuntu系统中，代理只能在浏览器中使用，之前每次都以为是订阅的问题，之后在终端中测试才发现没有使用代理，因此下面记录一下代理设置的过程：

可以直接在文件中修改，规则在`.gitconfig`文件中

```shell
vim ~/.gitconfig
```

之后添加相应规则，具体规则和端口号可以通过梯子软件的`复制终端代理命令`查看，如果没有也可以通过设置中查看，因为区别就在于不同协议的端口号不一样。

如果只需要在当前终端使用代理，仅需要使用一下命令：

```shell
export HTTP_PROXY=http://127.0.0.1:58591
export HTTPS_PROXY=http://127.0.0.1:58591
export ALL_PROXY=socks5://127.0.0.1:51837
```

使用命令设置设置代理：
```shell
// 使用http/https代理
git config --global http.proxy http://127.0.0.1:58591
git config --global https.proxy http://127.0.0.1:58591

// 使用socks5代理
git config --global http.proxy socks5://127.0.0.1:51837
git config --global https.proxy socks5://127.0.0.1:51837
```

直接在`.gitconfig`文件中修改，直接添加：
```
[http]
        proxy = http://127.0.0.1:58591
[https]
        proxy = http://127.0.0.1:58591
```

参考链接：[Git设置代理](https://www.jianshu.com/p/739f139cf13c)



## 如何在Ubuntu上安装OpenVPN

参考连接：[Installation for Debian and Ubuntu](https://openvpn.net/cloud-docs/openvpn-3-client-for-linux/)

1. 打开终端`ctrl + alt + T`
2. 安装https传输包，`sudo apt install apt-transport-https`
3. 下载openvpn的key，`sudo wget https://swupdate.openvpn.net/repos/openvpn-repo-pkg-key.pub`。
   
   但是这里由于终端没有开代理，因此一直下载不下来，我们需要在终端中加入代理名称（需要使用自己的梯子的代理命令，如果没有梯子就没办法了），`sudo wget -e "HTTPS_PROXY=http://127.0.0.1:58591" https://swupdate.openvpn.net/repos/openvpn-repo-pkg-key.pub`
4. 验证添加key，`sudo apt-key add openvpn-repo-pkg-key.pub`
5. 下载安装OpenVPN包，`sudo wget -e "HTTPS_PROXY=http://127.0.0.1:58591" -O /etc/apt/sources.list.d/openvpn3.list https://swupdate.openvpn.net/community/openvpn3/repos/openvpn3-focal.list`
   
   这里需要注意同样需要在wget中使用代理才能顺利下载。
   同时需要修改的是对应系统的版本代号，具体系统的代号参见上面的参考链接，我使用的Ubuntu20.04代号为`focal`。
6. 接下来使用`sudo apt update`和`sudo apt install openvpn3`，这里可能也需要使用代理，中间加入`-o Acquire::https::proxy="http://127.0.0.1:58591"`选项即可。

## Ubuntu上使用Openvpn3

- 建立会话：
```shell
# 我这里是openvpn.opvn文件
openvpn3 session-start --config ${CONFIGURATION_PROFILE_NAME}
```

- 管理会话：
```shell
# 查看连接
openvpn3 sessions-list
# 通过配置文件管理会话
openvpn3 session-manage --config ${CONFIGURATION_PROFILE_NAME} --restart
openvpn3 session-manage --config ${CONFIGURATION_PROFILE_NAME} --disconnect
```
其他命令可以通过`--help`获得详细说明。

## 如何生成项目中的目录树

通过`mddir`这个包，具体步骤为：
1. 安装`npm`
   
   ```shell
   sudo apt-get install npm
   ```
2. 安装`mddir`
   
   ```shell
   sudo npm install mddir -g
   ```
3. 生成目录树
   
   ```shell
   cd dir
   mddir
   ```

在所在目录下，就会有`directoryList.md`文件。


