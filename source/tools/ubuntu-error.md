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


