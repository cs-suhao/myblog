## 1.6 系统调用
操作系统为用户程序提供了一系列基本功能的接口，称为系统调用，一般系统调用分成4种：

### 1.6.1 进程管理

- `pid = fork()`
  
  创建与父进程相同的子进程。创建一个原有进程的精确副本，包括所有的文件描述符、寄存器，如果传入的参数不同，两个进程会执行不同的过程。

- `pid = waitpid(pid, &statloc, options)`
  
  等待一个子进程终止。

- `s = execve(name,argv,environp)`
  
  替换一个进程的核心映像。

- `exit(status)`
  
  终止进程并返回状态。


上述进程管理的系统调用我们可以用一个shell程序理解：

```c
// 1
while(true){ 
    // 2
    typr_prompt();
    // 3
    read_command(command,parameters);

    // 4
    if(fork()!=0){
        waitpid(-1,&status,0);
    }else{ // 5
        execve(command,parameters,0);
    }
}
```
解释：
1. 这里是让shell程序一直运行
2. 在屏幕上显示提示符
3. 从终端读取输入
4. 如果fork()!=0，则该进程是父进程，会等待子进程结束
5. 如果fork()==0，则该进程是子进程，会执行输入的命令



### 1.6.2 文件管理

- `fd = open(file,how,...)`
  
  打开一个文件供读/写。

- `s = close(fd)`
  
  关闭一个打开的文件。

- `n = read(fd,buffer,nbytes)`
  
  把数据从一个文件读到缓冲区。

- `n = write(fd,buffer,nbytes)`
  
  把数据从缓冲区写到一个文件中。

- `position = lseek(fd,offset,whence)`
  
  移动文件指针。

- `s = stat(name,&buf)`
  
  取得文件的状态信息。包括文件类型、大小、最后修改时间等。

### 1.6.3 目录和文件系统管理

- `s = mkdir(name,mode)`
  
  创建一个新目录。

- `s = rmdir(name)`
  
  删除一个空目录。

- `s = link(name1,name2)`
  
  创建一个新目录name2，并指向name1。这里的link很像共享，一个用户所作的操作，别的用户也可以看见。

- `s = ublink(name)`
  
  删去一个目录项。

- `s = mount(special,name,flag)`
  
  安装一个文件系统。

- `s = umount(special)`
  
  卸载一个文件系统。

### 1.6.4 杂项

- `chmod`
  
  改变文件的保护模式。