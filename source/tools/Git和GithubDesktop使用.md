---
title: Git和GithubDesktop使用
mathjax: false
date: 2021-11-03 10:26:12
tags:
---



使用Git、Github Desktop和VS Code管理和编辑项目。

<!-- more -->

# Git和Github Desktop使用

使用Git命令固然可以，但是在不同的系统上进行推送，每次都需要翻墙并`git pull`和`git push`，有时候容易出现问题，需要查命令来解决，所以我决定使用`Github Desktop`，并使用`VS code`写代码，两者结合进行项目代码的更新。

## 下载与安装

### Windows/Mac

这两个系统直接搜索`Github Desktop`，官网上提供了对应的安装程序，下载安装即可。

### Linux

从`Github`仓库中下载`.deb`包进行安装即可，链接如下：

[shiftkey/desktop: Fork of GitHub Desktop to support various Linux distributions](https://github.com/shiftkey/desktop)

## 创建第一个仓库

- **Create a tutorial repository and clone it**

  这一步会首先让你输入`Private Acess Token`也就是`Github`账户密码，输入完成后，创建仓库完成。

- **Install a text editor**

  选择默认IDE，因为我已经安装了VS code，因此这一步直接默认完成。

- **Create a branch**

  这里教程要求创建一个分支，分支是为了将整体修改流程分叉保存，也就是说多个人可以对项目进行不同的修改而不受他人影响，他们在自己的分支上修改。之后也可以合并，最终合并到`master`也就是主分支上，用于定位错误。

- **Edit a file**

  教程要求打开VS code，并在`README.md`的`line 6`写自己的名字，并返回`Github Desktop`的GUI页面。

- **Make a commit**

  返回GUI界面可以看到，在中间窗口已经显示了修改的地方，左侧`Changes`一栏也可以看到`1 changed file`提示，需要在下方的`Summary`和`Description`输入相应修改的说明，其中`Summary`是必须的。

- **Publish to Github**

  `commit`以后，点击**Publish  branch**

- **Open a pull request**

  这里的`open a pull request`是针对多个分支进行的，之前我们创建了一个`update`分支，但是该分支还没有合并到主分支中，这里的`pull request`就是让该分支请求合并到主分支中，由于本教程只更新了`README.md`，所以直接就合并了。

> 之后可以在`Github`中删除该仓库，删除需要将仓库名称完整打一遍。

## 远程克隆一个仓库并用VS code修改

- **Clone a repository from the Internet**

  可以选择自己的`repository`，也可以填写别人仓库的`url`，下方需要设置`Local path`。

- **Write your code and change file**

  写代码，对项目进行修改

- **Commit and Push**

  返回`Github Desktop`进行`commit`和`push`操作

## 克隆远程仓库并放入自己的Github中

首先需要分清楚两个概念：**`fork`**和**`git clone`**的区别，**`fork`**代表复制他人的一个项目到自己的`github`中，之后这个项目的地址会是你自己的`github`地址，而在本地仓库是没有任何改变的。

而**`git clone`**则是将别人的代码完完整整下载到本地仓库中，但是如果你不是项目的开发人员，在本地所做的所有修改，是无法`push`到仓库中的，因为在该项目的`git`文件中还是别人的项目，你是无权修改的。

所以想要克隆别人的仓库并在自己的Github中进行修改，需要先`fork`，再`git clone`，之后进行的`git add`、`git commit`都是常规操作。


>- **`fork`**`Gitee`中的仓库到`Github`中
>  1. 尝试使用`import old repositories`，但是不知道是`Gitee`的原因还是墙的原因，总是无法导入成功。
> 2. 我尝试使用`fork`将仓库复制到自己的`gitee`中，再通过Github Desktop`git clone`，查看是否可行。

> 经过尝试，在`Github Desktop`中是可以`clone`存在`Gitee`中的仓库的。
>- 直接在本地进行修改

上述方法可以抛弃了，因为在Github Desktop官方例子中给出了方法，**使用GitHub Desktop添加现有项目到GitHub中**。

具体步骤如下：

1. 使用命令行，删除当前仓库配置的任何Git远程

   ```shell
   git remote -V
   git remote remove origin
   ```

   

2. 添加仓库到GitHub Desktop中

3. 点击**Publish Repository**，注意选择**Keep this code private**