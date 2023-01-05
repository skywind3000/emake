## Preface

GNU Make 太麻烦？Makefile 写起来太臃肿？头文件依赖生成搞不定？多核同时编译太麻烦？Emake 帮你解决这些问题：

- 使用简单：设定源文件，设定编译参数和输出目标就行了，emake为你打点好一切。
- 依赖分析：快速分析源代码所依赖的头文件，决定是否需要重新编译。
- 输出模式：可执行、静态库（.a）、动态库（.so/.dll）。
- 多核编译：轻松实现并行编译，加速项目构建。
- 精简紧凑：只有唯一的一个 emake.py 文件。
- 交叉编译：构建 iOS 项目 ，安卓项目，等等。
- 语言支持 `C` / `C++` / `ObjC` / `ObjC++` / `ASM`
- 工具支持 `gcc` / `mingw` / `clang` 
- 运行系统 `Windows` / `Linux` / `Mac OS X` / `FreeBSD` 
- 信息导出：项目文件列表，目标文件，以及 `compile_commands.json` 等。
- 方便的交叉编译，轻松构建 `Android NDK` / `iOS` / `asm.js` 项目
- 你见过最简单的构建系统，比 Gnu Make / CMake 都简单很多

只有两三个源代码，那 makefile 随便写，文件一多，搞依赖都可以搞死人。emake 就是简单中的简单，不但比 GNU Make 简单，还要比 cmake 简单很多。

应为 CMake 和 GNU Make 都是**命令式构建工具**，而 emake 是**定义式构建工具**，命令式当然可以处理各种复杂情况，本身就是一门编程语言，强大却失之复杂，而定义式类似 IDE 那样，设定文件，编译参数链接参数，就能开始工作了，虽然做不到命令式那么灵活，却能满足大多数中小型项目开发，个人实验项目开发。

Emake 是为快速开发而生的，最初版本在 2009年发布，多年间团队在不同操作系统下用它构建过：服务端项目、客户端项目、iOS项目、安卓项目 和 Flash项目，这些项目都稳健的跑在生产环境中，为海量用户提供服务。

多年的开发中，emake 提高了各种大小项目的开发效率，自身也随着时间增加不断被完善和稳定。


## Install

#### Linux / Mac OS X: 

```bash
wget http://skywind3000.github.io/emake/emake.py
sudo python2 emake.py -i
```

运行上面两条指令，十秒内完成安装。emake 会拷贝自己到 /usr/local/bin 下面，后面直接使用 emake 指令操作。

#### Windows:
	
下载 emake.py，放到你的 mingw 根目录下（便于 emake 定位 gcc），并且添加到 PATH 环境变量，同级目录新建立一个 emake.cmd 文件，内容如下：

```batch
@echo off
d:\dev\python27\python.exe d:\dev\mingw\emake.py %*
```

修改一下对应路径即可，建立这个 emake.cmd 的批处理文件是为了方便每次敲 emake 就可以工作，避免敲 "python emake.py" 一长串。

## Tutorial

假设你有三个文件：foo.c, bar.c, main.c 共同编译成名字为 main(.exe) 的可执行文件，我们创建 “main.mak” 文件：

```make
; 指明目标格式：exe, lib, dll 三选一
mode: exe

; 加入源文件
src: foo.c
src: bar.c
src: main.c

```

是不是比 makefile, cmake 之类的步骤简单多了？编译项目：

```bash
emake main.mak
```

好了，工程顺利编译成功，每次任何一个文件发生变动，相关对其依赖的源文件都会重新编译，而无依赖的代码则不需要再次编译。

#### 增加编译选项

如果需要增加编译选项的话：

```make
; 指明目标格式：exe, lib, dll 三选一
mode: exe

; 编译选项
flag: -Wall, -O3, -g

; 加入源文件
src: foo.c
src: bar.c
src: main.c

```

如果项目中使用了数学库 libm.a的话：

```make
link: m
```

如果还是用了 libstdc++.a 的话：

```make
link: m, stdc++
```

或者：

```make 
link: m
link: stdc++
```

link 可以直接写 .a 库的文件名：

```make
link: ./lib/libmylib.a
```

如果需要添加额外的 include 目录 和 lib 目录的话：

```make
inc: /usr/local/opt/jdk/include
lib: /usr/local/opt/jdk/lib
```

还可以手动指定输出的文件名：

```make
out: main
```

手动指定临时文件夹，避免临时 .o 文件污染当前目录的话：

```make
int: objs
```

这样所有的临时文件就会跑到 objs 目录下面了，想要清理的话，删除  objs目录即可。

#### 完整例子

```make
; 指明目标格式：exe, lib, dll 三选一
mode: exe

; 编译选项
flag: -Wall, -O3, -g

; 设定链接
link: m, pthread, stdc++

; 额外头文件路径
inc: /usr/local/opt/jdk/include
inc: /usr/local/opt/jdk/include/linux

; 额外库文件路径
lib: /usr/local/opt/jdk/lib

; 加入源文件
src: foo.c
src: bar.c
src: main.c

```

## Document

emake 的工程文件里面支持下面几种设置：

### src

用于声明项目里面的源文件，格式：

```make
src: file1
src: file2
...
src: filen
```

或者：

```make
src: file1, file2, file3
src: file4, file5, file6
```

### inc

声明项目中的 include 文件夹，相当于 gcc 的 -I 命令：

```make
inc: dir1
inc: dir2
```

或者：

```make
inc: dir1, dir2
```

和 src 一样可以使用逗号分隔。

### lib

设置库文件目录，格式同上

### link

添加需要链接的库，相当于 gcc 的 -l 指令：

```make
link: m, pthread, stdc++
```

或者：

```make
link: m
link: pthread
link: stdc++
```

同时支持单行和多行模式，编译 C++ 项目别忘记链接 stdc++。

### mode

目标文件的输出格式：

```make
mode: [exe|lib|dll|win]
```

- exe: 生成可执行文件
- lib: 生成静态链接库
- dll: 生成动态链接库
- win: windows下特有，生成无 console窗口的 windows程序。

### out

指定目标文件的文件名：

```make
out: target_file_name
```

### int

指定中间临时文件目录，一般设置为：

```make
int: objs
```

或者：

```make
int: objs/$(target)
```

### flag

指定编译参数，会被直接传递给 gcc.

```make
flag: -Wall, -g, -pg
```

诸如此类

### Events

根据条件运行不同 shell 命令：

```
# 加载前
preload: echo "preload"

# 编译前
prebuild: echo "prebuild"

# 链接前
prelink: echo "prelink"

# 编译后
postbuild: echo "postbuild"
```

其中 `preload` 和 `prebuild` 之间的区别是，`preload` 每次都会运行，并且是在解析项目文件和分析依赖之前运行，而 `prebuild` 是在解析项目文件分析依赖之后运行，如果二进制没更新，`prebuild` 就不会被调用，而 `preload` 每次都无条件被执行。

比如某个源代码是使用其他工具生成的，放到 `prebuild` 时，应为处于依赖分析之后，初次构建尚未触发生成代码前面依赖分析就会报找不到文件了，但 `preload` 在依赖分析之前运行。


## Settings

Emake 可以指定一个 ini 文件来进行全局配置：

原来是：
	
	emake <parameters>

手动指定配置文件名：

	emake --ini=xxx.ini <parameters> 

如果不指明的话，会首先在当前文件夹寻找 emake.ini 文件，同时 Linux 下面的话，还会相继在下面三个位置：

	/etc/emake.ini
	/usr/local/etc/emake.ini
	~/.config/emake.ini

两外一种指定配置的方式是 `--cfg=name` 方式：

    emake --cfg=mingw64 <parameters>

这样它会去试图加载 `~/.config/emake/{name}.ini` ，方便你集中管理不同的全局配置文件。

进行寻找。该配置文件确定了一些编译的默认配置，在该配置文件中，可以：

- 更改默认编译器的可执行文件名
- 更改默认连接器的可执行文件名
- 设定编译条件
- 设定默认编译的参数：include / lib 等文件夹等
- 设定编译器启动的一些环境变量
- 设定多核编译时的 cpu 数量。
- 预先设定一些 section，工程文件可以 import 特定的 section。

由上面这些设定，emake 可以灵活的调用各种工具链，方便的进行项目构建和交叉编译。比如我在 Windows 下面的 emake.ini 部分内容：

```ini
[default]
flag=-Wall, -I$(inihome)/../mylibs
link=stdc++, winmm, wsock32, opengl32, gdi32, glu32, ws2_32, user32

include=d:/dev/local/include
lib=d:/dev/local/lib

cpu=6

[ffmpeg]
include=d:/dev/local/opt/ffmpeg/include
lib=d:/dev/local/opt/ffmpeg/lib
link=avcodec, avdevice, avfilter, avformat, avutil, postproc, swscale


[qt]
include=D:/Dev/Qt/sdk/4.8.3-mingw/include;D:/Dev/Qt/sdk/4.8.3-mingw/include/QtGui
lib=D:/Dev/Qt/sdk/4.8.3-mingw/lib
link=stdc++, ole32, gdi32, wsock32, opengl32, gdi32, glu32, ws2_32, uuid, oleaut32, winmm, imm32, winspool, QtCore4, QtGui4, QtGuid4

[qt45]
include=D:/Dev/Qt/4.5.0-mingw-static/include;D:/Dev/Qt/4.5.0-mingw-static/include/QtGui
lib=D:/Dev/Qt/4.5.0-mingw-static/lib
link=stdc++, ole32, gdi32, wsock32, opengl32, gdi32, glu32, ws2_32, uuid, oleaut32, winmm, imm32, winspool, QtCore, QtGui


```

默认区（default）作用于每一个工程文件，其中 cpu字段只能出现在默认区，它规定了编译时最多使用多少个核进行编译，其他区的话，需要在工程里使用 import 来导入：

	import: qt, ffmpeg

那么在你的工程里，上面 qt 和 ffmpeg 的相关配置就会被导入了。


## Cross Compilation

交叉编译的话，需要单独一个 ini 文件来规定工具链，比如我的 android交叉编译配置：

```ini
[default]
flag=-Wall
home=bin
gcc=arm-linux-androideabi-gcc
ar=arm-linux-androideabi-ar
as=arm-linux-androideabi-as
name=android,posix,arm
cpu=4

```

其中 home 规定了 ndk 工具链 gcc 环境所在的可执行路径，可以是绝对路径，或者是相对于 ini 配置文件的相对路径，后面同时定义了：gcc, ar, as 三个必须的可执行文件名，使用的时候：

	emake --ini=d://android-toolchain/android-9/emake.ini xxx

或者放到 `~/.config/emake/android-9.ini` 里面，用：

    emake --cfg=android-9 xxx

指定使用它，在 default 区中定义了很多 name ，这些 name 可以用来做工程文件的条件判断，比如：

	android/flag: -mfloat-abi=softfp
	posix/link: pthread
	win32/link: winmm, wsock32, ws2_32

不同的 ini 文件中定义的 name 不同，在工程文件中会判断是否定义过某个 name ，定义过的话，执行后面的话，如此在同一个工程文件中，可以针对不同平台定义源文件，设置编译参数。

## Rapid Development

不管时 GNU Make 还是 cmake，亦或时其他构建系统，都需要你写一个专门的工程文件来描述该工程。对于大项目很正常，但是对于中小项目，特别时一些测试类项目，这真的太麻烦了。

Emake 可以不用工程文件，而将工程配置信息嵌入到源代码的注释中：

```cpp
#include <stdio.h>
#include <stddef.h>
#include "foobar.h"


//! mode: exe
//! src: foo.cpp, bar.cpp, utils.cpp
int main(void)
{
	printf("Hello, World !!\n");
	foo();
	bar();
	return 0;
}

```

这样在你的源文件里面增添两行以后，即可使用：

	emake main.cpp

来进行编译，emake 会自动提取 `//!` 开头的注释，解析为 emake的项目描述信息，上面的配置描述了该项目依赖的文件（除了 main.cpp自己外），以及项目模式为生成可执行文件。

这样写起来，比所有构建系统都简单很多。

## 输出信息

列出项目最终输出文件，比如 `xxx.so` 之类：

```bash
emake -o hello.mak
```

列出项目所包含的源文件：

```bash
emake -list hello.mak
```

列出编译参数：

```bash
emake -cflags hello.mak
```

列出依赖：

```bash
emake -depends hello.mak
```

输出 `compile_commands.json` 内容:

```bash
emake -commands hello.mak
```

还有更多信息，具体见：`emake -h` 说明。

## Credits

本项目旧地址：

https://code.google.com/archive/p/easymake/

