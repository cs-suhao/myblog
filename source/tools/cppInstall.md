# Windows上安装C++
## 所需软件
- [VS Code]()
- [MinGW](https://sourceforge.net/projects/mingw-w64/)

## 安装步骤
1. 安装VS Code和相关插件，具体参考[官网](https://code.visualstudio.com/docs/cpp/config-mingw)。
2. 安装MinGW-w64，参数选择如下：
    - Architecture: 64位选x86_64
    - Threads: posix跨平台好，win32性能好
    - Exception: seh支持64位并且性能好