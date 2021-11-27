
由于学长的on-VHLL项目代码有用到Conan管理C＋＋库，同时我在自己的Ubuntu上安装使用的时候出现了非常多的问题，因此就去翻看了Conan的官方文档，现总结学习一下。

<!-- more -->
# Conan库学习
## Conan介绍
Conan是一款免费开源、跨平台的C和C++依赖与包管理器，可以和不同编译器协作。并且Conan可以针对不同平台、不同架构、不同编译器版本管理不同版本的二进制包，并且可以让用户自己上传并管理自己的依赖包。

Conan的整体架构是`client-server`，和`git`使用模式非常相似。服务器端仅用于包存储，不能build或create包，包是由client生成并上传的，并且如果二进制文件是由client build，那么编译过程同样是由client端的应用完成。Conan由以下三个部分组成：
- Conan client: 终端应用。
- CE: 推荐的用于管理个人私有的包的Conan服务器。
- ConanCenter: 是中央公共包服务器，用于存储开源并且流行的库。

Conan的强大特点在于其可以生成并管理针对任意平台和设置的预编译二进制文件，可以节省从`source`生成的时间。

## Conan Cheat Sheet
待更新



## Conan安装
### pip安装
文档中推荐使用`pip`进行安装，但是我在Ubuntu18.04安装过程中遇到了一些问题，会在下面进行记录。

官方文档的安装步骤如下：
- 安装Python，且Python版本>=3.5
- 使用pip进行安装
```shell
# pip升级
pip install --upgrade pip
pip --version
sudo pip install conan
```
官方文档中说Linux系统中安装需要用`sudo`权限，进行全局安装，这可能也是我之前没有成功的原因。
查看是否安装成功使用以下命令：

```shell
conan --version
```
我在使用`pip`安装之后一直提示*command not found*，之后排查的原因也是因为没有把Python包的安装路径放在全局，因此运行以下命令解决：
```shell
python3 -m site
```
从中找到`USER_SITE`的目录，我的是`/home/sh/.local/lib/python3.9/site-packages`，其中可以看到conan已经安装了，之后将conan的路径放在`bashrc`中：
```shell
echo PATH=${PATH}:/home/sh/.local/lib/python3.9/site-packages >> ~/.basgrc
source ~/.bashrc
```
官方的建议用以下命令：
```shell
source ~/.profile
```
这一点在我第一次安装的时候并灭有发生，但是如果出现问题，如果不是python版本问题，基本上就是路径不是全局的，因此针对这个问题只需要执行相应操作即可。

### source安装
官方文档还直接给出了从源码安装的方法，不需要安装Python和pip，使用如下命令：
```shell
git clone https://github.com/conan-io/conan.git conan_src
cd conan_src
sudo python -m pip install -e .
conan
```

## Conan使用实例（md5.cpp）

在重新走安装流程之后，注意以下几点，目前跑官方提供的实例并没有出现问题：

- Ubuntu18.04在自动更新的时候，会自动更新Python3.6版本，因此不需要自己再安装Python3
- 安装好`git`
- 注意使用`root`权限和`python3`安装`conan`包
- 先不要修改`default`配置，在进行实例的过程中，会自动创建`default`文件，之后可以修改

### An MD5 hash calculator using the Poco Libraries

直接`git clone`例程库

```shell
$ git clone https://github.com/conan-io/examples.git && cd examples/libraries/poco/md5
```

在这个文件中会有已经写好的`md5.cpp`文件，可以用`cat`命令查看，之后由于该应用依赖于`Poco`库，所以可以在`ConanCenter`远程服务器中查看，搜索相应版本的命令如下：

```shell
conan search poco --remote=conancenter
```

同时查看相应版本的`reference`可以使用如下命令

```shell
conan inspect poco/1.9.4
```

在使用`Conan`进行包安装时，会使用`conanfile.txt`，该文件会放在`cpp`文件同一级文件下，里面写明白需要安装的相关库以及版本。其中，该例子的`conanfile.txt`内容如下：

```
 [requires]
 poco/1.9.4

 [generators]
 cmake
```

其中关于`requires`依赖包的写法，可以在后面加`@`和`user-chanel`，但是需要在远程服务器中找到相应的依赖包版本，否则会出现错误。

之后官方文档会说明**GCC compiler >= 5.1**，因为`conan`默认设置编译器链接到旧的ABI上，因此需要以下两条命令修改，使其支持`c++11`，命令如下：

```shell
conan profile new default --detect  # Generates default profile detecting GCC and sets old ABI
conan profile update settings.compiler.libcxx=libstdc++11 default  # Sets libcxx to C++11 ABI
```

之后进行编译和安装

```shell
mkdir build && cd build
conan install ..
```

在这一步文档之提示，超过130种不同配置二进制库会在ConanCenter中，但是如果自己的当前配置不是预置在ConanCenter中的，会产生**BinaryMissing**，如果遇到这种情况需要使用`conan install .. --build=missing`这个命令。

之后在`build`文件夹中写入相关`cmake`信息，`CMakeLists.txt`内容如下：

```
 cmake_minimum_required(VERSION 2.8.12)
 project(MD5Encrypter)

 add_definitions("-std=c++11")

 include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
 conan_basic_setup()

 add_executable(md5 md5.cpp)
 target_link_libraries(md5 ${CONAN_LIBS})
```

之后进行编译运行：

```shell
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## 安装师兄On-vHLL-Code出现问题

### Error 1
在后续的使用中，出现了`gtest/1.8.1 unable found in remotes`的问题，也就是包在远程服务器中找不到，但是同门之前在使用中，顺着流程走下来，一点问题都没有，因此我进行了一些尝试：
1. **添加新的conan-center**
   
   但是gtest包我在conancenter的网站中找到了相应的版本，所以remote源的问题排除。

   > ！！！实际上这里并没有排除，在后续的操作中，我发现gtest/1.8.1版本是可以找到的，但是并不能找到gtest/1.8.1@bincrafters/stable。
   
2. **检查conan的配置信息，以防有误**

   我针对`gtest/1.8.1`到ConanCenter找，结果发现配置信息都是最基础的，并且在官方给的例程中，自动生成的`default`文件中的配置是一摸一样的，因此排除配置信息的问题。

3. **直接安装gtest/1.8.1**

   太麻烦就没有尝试，而且明显是conan的问题，而不是gtest本身的问题。

### Error 2


```shell
ERROR: Error loading conanfile at '/home/sh/.conan/data/poco/1.9.4/_/_/export/conanfile.py': Unable to load conanfile in /home/sh/.conan/data/poco/1.9.4/_/_/export/conanfile.py
  File "/home/sh/.conan/data/poco/1.9.4/_/_/export/conanfile.py", line 97
    tools.get(**self.conan_data["sources"][self.version], 
    												^
SyntaxError: invalid syntax
```

根据Google查询得知，这个错误是因为Python2无法解析```self.conan_data```，需要使用Python3，回答者给出的建议是卸载python2所安装的conan，使用python3安装的conan，这一点有点坑，因为官方文档中，给出的命令是`python`，而`python`默认就是Python2。

```shell
python2 -m pip uninstall conan
python3 -m pip install -U conan
```

但是这里又出现了问题，因为我使用源码进行安装，所以在通过`pip`卸载的时候，`pip`是找不到的，可以查询源码安装的Conan如何卸载。这样一看还是不如直接使用`pip3`安装实用，因此针对这个错误我决定重新走安装流程。

## 上述问题解决方案

### 问题1解决

针对`gtest/1.8.1`在`remote`找不到的问题，由于一开始没有注意`@`后面跟着的`user-chanel`，因此在`conancenter`这个远程库中查询是可以找到`gtest/1.8.1`，但是后来我注意到在这个远程库中找不到带`@`的对应包，因此查询官方文档发现，Conan针对远程库有过一次更新，更换了新的ConanCenter，内容如下：

> **ConanCenter** (https://conan.io/center) is the main official repository for open source Conan packages. It is configured as the default **remote** in the Conan client, but if you want to add it manually:
>
> ```shell
> conan remote add conancenter https://center.conan.io
> ```
>
> **conan-center** was the official repository but is **no longer a default \**remote\**** in the Conan client and **its usage is completely discouraged**. This documentation is kept here only for reference purposes.
>
> ```shell
> conan-center: https://conan.bintray.com [Verify SSL: True]
> ```
>
> **It contains all the packages that were uploaded to ConanCenter before July 1st (new packages are no longer uploaded to this remote), as well as legacy packages with full reference (zlib/1.2.11@conan/stable). **
>
> 上述说明，第二个远程库从7月以后不再更新，已经弃用。

所以我安装新版本的`Conan 1.42.0`会自动使用第一个链接，而同门在7月前安装，当时使用的还是第二个链接。

因此解决方案非常简单，使用如下命令，将第二个远程库的链接加入进去：

```shell
conan remote add conan-center https://conan.bintray.com
```

### 问题2解决

按照时间顺序，问题2应该是出现在问题1之前，由于我使用虚拟机重新安装，因此在系统更新以后，直接使用以下命令安装`conan`即可：

```shell
sudo python3 -m pip install conan
conan --version
```

