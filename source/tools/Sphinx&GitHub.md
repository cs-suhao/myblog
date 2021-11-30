# 基于Sphinx + GitHub + Read the Docs 搭建个人博客

在搜索技术文章时，被一个类API文档库的博客吸引，我觉得这种清晰的分类正是我需要的。之前采用Hexo+Github方式搭建，虽然有个人网站的意思，但是内网无法访问、目录分类不够清晰是我的痛点，因此看到这个博主搭建的，立刻去尝试了一下，该方案的技术路线是Sphinx发布在GitHub上，再通过Read the Docs构建公开文档库，实现公网发布。

此外，公开库的文档也可以由Sphinx自动生成，因此学习一下使用方法，在后续的开源库中撰写文档会有一些帮助。

## Sphinx安装及配置

直接使用pip命令进行Sphinx及其依赖的安装，命令如下：
```shell
pip3 install sphinx sphinx-autobuild sphinx_rtd_theme -i https://pypi.douban.com/simple/
```

创建博客文件夹，进入文件夹中进行初始化设置
```shell
sphinx-quickstart

> Project name: myblog
> Author name(s): ss
> Project release []: 1.0
> Project language [en]: zh_CN
```
之后在该文件夹中会出现以下文件夹\文件，解释如下：
- build: make生成文件存放目录
- source: 文档源文件存放目录
- Makefile: 用于编译生成静态html
- make.bat: bat脚本（一般用不着）

在`source/`文件夹中有`config.py`配置文件，我们主要就是修改该文件进行**扩展、页面等**设置。

具体配置可以见官方文档：

我的配置文件如下：
```python
import recommonmark
import myst_parser

project = 'Personal Bolg'
copyright = '2021, Suhao'
author = 'Suhao'

release = '1.0'

extensions = [
    # 'recommonmark',
    'sphinx_markdown_tables',
    'myst_parser'
]

templates_path = ['_templates']

language = 'zh_CN'

exclude_patterns = []

source_suffix = {
    '.rst': 'restructuredtext',
    '.md': 'markdown'
}

math_number_all = True

html_theme = 'sphinx_rtd_theme'

html_static_path = ['_static']
```
几乎就是默认版本，我修改了几句：
- import: 由于需要扩展，我在文件中加入了两个用于解析markdown文件的扩展，其实`recommonmark`和`myst_parser`只需要一个即可。
- extensions: 参照上述`import`进行扩展说明，另外由于上面已经导入过`recommonmark`，这里再加入会出错。
- source_suffix: 添加相应后缀，包括`.rst`和`.md`。
- math_number_all: 用于页面中嵌入公式。默认是`false`。
- html_theme: 网页风格，修改可以参见[官方文档](https://www.sphinx-doc.org/en/master/usage/theming.html#builtin-themes)

## GitHub托管

之后将`myblog`项目推送到自己的`github`仓库中，注意生成`.gitignore`文件，只需要忽略`build/`文件夹即可。

## Read the Docs生成文档

在Read the Docs网站中注册登录，可以直接使用github账号进行登录，选择`myblog`项目生成文档。

此外，由于Read the Docs的python环境没有`recommonmark`等包，构建的时候会出现错误，所以我们需要在`myblog/`目录下创建`requirements.txt`文件来说明依赖包。

`requirements.txt`内容如下：
```
sphinx-markdown-tables==0.0.15
myst-parser==0.15.2
```

之后每一次仓库有了新的推送时，Read the Docs都会自动拉取并进行构建。

## 文档撰写

写文档直接在对应的目录下新建`.md`即可，需要注意的是，如果`.md`文件中出现目录即`[TOC]`构建时会出现问题，所以不推荐使用，直接用一级二级标题，在网页中会自动进行归档。

如创建新文件夹，例如`c++/`，则需要在`source/`目录下的顶层`index.rst`修改，添加`c++/index`即可，同时在`c++/`文件夹中创建对应的`index.rst`，添加对应markdown文件。



> ## 参考：
> [Sphinx官方文档](https://www.sphinx-doc.org/en/master/contents.html)
> 
> [30 分钟搭建个人博客 | 最强的文档编排组合：Sphinx + Github + ReadTheDocs](https://iswbm.com/134.html)
> 
> [Sphinx + Read the Docs 从懵逼到入门](https://zhuanlan.zhihu.com/p/264647009)

