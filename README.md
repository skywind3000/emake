## Preface

GNU Make 太麻烦？Makefile 写起来太臃肿？头文件依赖生成搞不定？多核同时编译不好弄？Emake 帮你解决这些问题：

- 使用简单：设定源文件，设定编译参数和输出目标就行了，emake为你打点好一切。
- 依赖分析：快速分析源代码所依赖的头文件，决定是否需要重新编译。
- 输出模式：可执行、静态库（.a）、动态库（.so/.dll）。
- 多核编译：轻松实现并行编译，加速项目构建。
- 精简紧凑：只有唯一的一个 emake.py 文件。
- 干净利索：无需导出 Makefile/.sln 等中间文件，构建一步到位。
- 交叉编译：构建 iOS 项目 ，安卓项目，等等。
- 语言支持 `C` / `C++` / `ObjC` / `ObjC++` / `ASM`
- 工具支持 `gcc` / `mingw` / `clang` 
- 运行系统 `Windows` / `Linux` / `Mac OS X` / `FreeBSD` 
- 信息导出：项目文件列表，目标文件，以及 `compile_commands.json` 等。
- 包管理支持 `pkg-config`，`vcpkg` 和手工等三种方式导入第三方包。 
- 方便的交叉编译，轻松构建 `Android NDK` / `iOS` / `asm.js` 项目。
- 你见过最简单的构建系统，比 Gnu Make / CMake 都简单很多。

只有两三个源代码，那 makefile 随便写，文件一多，搞依赖都可以搞死人。emake 就是简单中的简单，不但比 GNU Make 简单，还要比 cmake 简单很多。

应为 CMake 和 GNU Make 都是**命令式构建工具**，而 emake 是**定义式构建工具**，命令式当然可以处理各种复杂情况，本身就是一门编程语言，强大却失之复杂，而定义式类似 IDE 那样，设定文件，编译参数链接参数，就能开始工作了，虽然做不到命令式那么灵活，但能满足大多数中小型项目开发，个人实验项目的日常开发。

Emake 是为快速开发而生的，通过牺牲了部分灵活性，却换来了极大的便利性，最初版本在 2009年发布，多年间团队在不同操作系统下用它构建过：服务端项目、客户端项目、iOS项目、安卓项目 和 Flash项目，这些项目都稳健的跑在生产环境中，为海量用户提供服务。

多年的开发中，emake 提高了各种大小项目的开发效率，自身也随着时间增加不断被完善和稳定。

## Content

- [Preface](#preface)
- [Content](#content)
- [Install](#install)
- [快速开始](#快速开始)
- [工程配置说明](#工程配置说明)
- [工具链配置](#工具链配置)
- [启动参数](#启动参数)
- [快速开发](#快速开发)
- [输出信息](#输出信息)
- [TODO](#todo)


## Install

#### Linux / Mac OS X

```bash
wget http://skywind3000.github.io/emake/emake.py
sudo python emake.py -i
```

运行上面两条指令，十秒内完成安装。emake 会拷贝自己到 /usr/local/bin 下面，后面直接使用 emake 指令操作。

#### Windows
	
下载 emake.py，放到你的 mingw 根目录下（便于 emake 定位 gcc），并且添加到 PATH 环境变量，同级目录新建立一个 emake.cmd 文件，内容如下：

```batch
@echo off
d:\dev\python311\python.exe d:\dev\mingw\emake.py %*
```

修改一下对应路径即可，建立这个 emake.cmd 的批处理文件是为了方便每次敲 emake 就可以工作，避免敲 "python emake.py" 一长串。

## 快速开始

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

#### 零工程文件

写工程文件还是觉得累？没关系，上面所有工程配置都可以用 **docstring** 的方式写在源文件里，使用 `//!` 开头的注释中，比如 main.cpp 里：

```cpp
#include <stdio.h>

//! mode: exe
//! flag: -Wall, -O3, -g
//! link: m, pthread
//! src: foo.c, bar.c
int main(void) {
    ...
    return 0;
}
```

源文件中 `//!` 开头的特殊注释会被 emake 提取，作为工程配置的内容。这个例子基本揽括了上面独立工程文件做的事情，当然 `src:` 部分 main.cpp 是会被自动加入的，不用额外写，然后构建时：

```bash
emake main.cpp
```

即可，简单到了极点，特别适合验证一些小想法，做一些小实验，不用写一大堆乱七八糟的东西。

#### 绝对路径

为了避免有时编译错误输出时，文件名用的相对路径，造成外部编辑器无法正确解析，可以用 `--abs` 参数：

```bash
emake --abs main.cpp
```

这样错误输出中的文件名，就是绝对路径了。

#### 使用技巧

- [如何 vcpkg 集成到 emake 中？](https://github.com/skywind3000/emake/wiki/%E5%A6%82%E4%BD%95-vcpkg-%E9%9B%86%E6%88%90%E5%88%B0-emake-%E4%B8%AD%EF%BC%9F)
- [如何进行交叉编译?](https://github.com/skywind3000/emake/wiki/%E5%A6%82%E4%BD%95%E9%85%8D%E7%BD%AE%E4%BA%A4%E5%8F%89%E7%BC%96%E8%AF%91%EF%BC%9F)
- [如何在 Windows 项目中添加 RC 文件？](https://github.com/skywind3000/emake/wiki/%E5%A6%82%E4%BD%95%E5%9C%A8-Windows-%E9%A1%B9%E7%9B%AE%E4%B8%AD%E6%B7%BB%E5%8A%A0-RC-%E6%96%87%E4%BB%B6%EF%BC%9F)

更多技巧见 [常见问题集](https://github.com/skywind3000/emake/wiki/Frequently-Asked-Questions)。


## 工程配置说明

Emake 的工程文件里面支持下面几种核心设置：

| 名称 | 含义 |
|-|-|
| src | 指定项目源文件，逗号分割，支持通配符 |
| inc | 指定 include 目录，逗号分割，支持相对路径（相对于工程文件）|
| lib | 指定库文件目录，格式同上 |
| link | 指定库文件，逗号分割，比如 `lib: png` 就会连接 `libpng.a` |
| flag | 指定编译通用选项，逗号分割 |
| cflag | 指定 C 语言的编译选项，逗号分割 |
| cxxflag | 指定 C++ 的编译选项，逗号分割 |
| define | 定义宏，格式为 `define: USE_UTF8=1`，可用逗号间隔多个宏 |
| mode | 设置目标格式：`exe`，`lib`, `dll`, `win` 几种 |
| out | 设置目标文件名 |
| int | 设置临时目录，放置 `.o` 文件和中间文件 |
| flnk | 连接时传入的参数，逗号分割 |
| wlnk | 连接时使用 `-Wl,` 前缀直接透传给 `ld` 的参数，逗号分割 |

还支持几种辅助配置：

| 名称 | 含义 |
|-|-|
| export | 当 mode 为 dll 时导出符号成 `.lib` 文件给 MSVC 用 |
| import | 从 emake.ini 配置的非 `default` 区导入配置 |
| package | 从 pkg-config 导入某个包配置，并设置 inc/lib 目录和 link 选项 |
| pcflag | 调用 pkg-config 时的参数 |
| echo | 输出文字 |
| color | 设置 echo 的颜色 |
| preload | 事件：加载前，后接 shell 命令 |
| prebuild | 事件：编译前，后接 shell 命令 |
| prelink | 事件：连接前，后接 shell 命令 |
| postbuild | 事件：构建后，即连接成功了就调用，后接 shell 命令 |
| environ | 设置环境变量 |

下面对其中几项略作说明。

#### 添加源代码

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

也可以带通配符：

```make
src: core/*.c
src: source/*.cpp
```

#### 目录设置

声明项目中的 include 文件夹，相当于 gcc 的 `-I` 命令：

```make
inc: dir1
inc: dir2
```

或者：

```make
inc: dir1, dir2
```

和 src 一样可以使用逗号分隔；而 `lib` 选项用于设置库文件目录，相当于 gcc 的 `-L` 命令，格式和 `inc` 类似。

#### 连接静态库

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

#### 目标格式

目标文件的输出格式：

```make
mode: [exe|lib|dll|win]
```

- exe: 生成可执行文件
- lib: 生成静态链接库
- dll: 生成动态链接库
- win: Windows 下特有，生成无 console 窗口的 Windows 程序。

#### 临时目录

指定中间临时文件目录，一般设置为：

```make
int: objs
```

或者：

```make
int: objs/$(target)
```

指定了以后，可以避免项目中间文件污染当前目录。

#### 条件编译

在 emake.ini 中可以指明一系列 name，比如：

```ini
[default]
name=android,posix,arm,nossl
```

每个名字代表一个条件，可以同时定义多个条件，然后在工程文件里使用：

```text
<name>/option: value
```

来声明，只有 emake.ini 的 name 里包含特定内容时，才会触发后面内容，比如：

```make
win32/link: pdcurses_wincon
linux/link: ncurses, tinfo
arm/src: arm_calculate.c
```

这两条语句代表不同平台 link 不同的库，`name` 可以不定义，它的默认值是 target，你可以只定义一个 target 不定义 name：

```ini
[default]
target=android
```

这样手工指明目标平台的名称，默认的话，会使用 python 里的 `sys.platform` 返回值。同时你又没有定义过 `name` 项目，那么 `name` 的默认值就是 `target`，除非你和上面一样手工定义 `name`，就能覆盖默认值。

所以 `name` 只有一个时，一般不定义，用默认值即可，但有多个 `name` 时需要定义下。

#### 编译配置

在启动 emake 时可以设置一个 `--profile` 参数来指定 debug/release 等不同的构建配置：

```bash
python emake.py --profile=debug main.mak
```

而在工程文件里，用 `@` 符号来指定某个配置是属于什么 profile 的，比如：

```make
flag@debug: -g, -Og
flag@release: -O2
flag@minsize: -Os
flnk@minsize: -s
```

那么 emake 就会根据命令行传入的 profile 选择对应的配置项目，再比如，给不同配置规定不同的目标文件名：

```make
win32/out: rogue-clone.exe
win32/out@debug: rogue-cloned.exe
linux/out: rogue-clone
linux/out@debug: rogue-cloned
```

这里分别演示了在不同平台下，不同 profile 可以指定不同的配置。

#### 细粒度参数

Emake 支持为每个源代码文件设置不同的编译参数，格式是就是在 `src` 定义完源文件后，右边加冒号跟着参数即可：

```make
src: main.c : -O3
src: foo.c, bar.c : -DTEST
```

上面第一行单独为 `main.c` 设置了 `-O3` 的参数，第二行为另外两个源文件定义了一个叫做 `TEST` 的宏。

#### 事件机制

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


## 工具链配置

Emake 支持多个工具链，每个工具链使用一个 ini 进行描述，不显示指定工具链的名字的话，会到下面几个位置寻找默认工具链配置：

1）在 `emake.py` 同级目录查找 `emake.ini` 。
2）/etc/emake.ini 
3）/usr/local/etc/emake.ini
4）~/.config/emake.ini

默认工具链配置并不强制，没提供的话，Emake 也会到 `$PATH` 中寻找 gcc 等工具并自动设置。

如果想用别的工具链配置的话，使用 `--cfg=name` 参数就会加载 `~/.config/emake/{name}.ini` ：

    emake --cfg=mingw64 <parameters>

这样它会去试图加载 `~/.config/emake/mingw64.ini`，也可以用 `--ini=path` 直接给 ini 文件绝对路径：

	emake --ini=/absolute/path/to/name.ini <parameters> 

通常使用 `--cfg=name`，并在 `~/.config/emake` 目录下统一管理所有配置，比如你有多套工具链，每个工具链一个配置放进去，用起来比较方便，特别是有交叉编译的情况时。

#### 配置格式

工具链配置文件的内容类似：

```ini
[default]
# 工具链的 bin 目录，用于查找 gcc / clang 等工具
home=d:/msys32/mingw32/bin
# 当你有多套工具链时，不可能都加入 $PATH，这个配置可以让 emake 在
# 构建时临时追加到 $PATH 前面，不污染外层父进程的环境变量
path=d:/msys32/mingw32/bin,d:/msys32/usr/bin

# 通用配置，免得每个工程文件写一遍
flag=-Wall
link=stdc++, winmm, wsock32, user32, ws2_32
cflag=-std=c11
cxxflag=-std=c++17

# 针对 debug/release/static 三种 profile 的设置，使用
# emake --profile=<name> xxx 在构建时指明使用啥 profile
define@debug=_DEBUG=1
define@release=_RELEASE=1
define@static=_STATIC=1, _RELEASE=1

flag@debug=-Og, -g, -fno-omit-frame-pointer
flag@release=-O3
flag@static=-O3, -static

# 多核编译
cpu=4

# 目标平台名称，不提供得话默认用 python 的 sys.platform 字符串代替
target=win32

# 条件编译时候的条件变量，在工程文件里可以用 win32/flag: xxx 来使用
name=win32,nt,have_openssl
```

配置都包括在 ini 的 `[default]` 区，其中第一行使用 `home` 定义了工具链的 `bin` 目录，在哪里将找到 `gcc`, `ar`, `as`, `ld` 等工具，可以用绝对路径，也可以用相对于 ini 文件的路径。

接下来使用 `path` 定义了调用工具链前需要把哪些目录追加到系统变量 `$PATH` 前，使用逗号分割。

中间就是一些通用参数，和针对 `debug`, `release` 和 `static` 三种 profile 的不同设置，然后用 `cpu=4` 指定了同时使用四核编译。

最后用 `target` 声明目标平台的名字是 `win32`。

这样的 ini 配置文件除了上面默认的位置加载外，还可以手工指定。


#### 配置项目

工具链配置文件的 `[default]` 区支持下面几种配置：

| 名称 | 含义 |
|-|-|
| home | 工具链的 bin 目录，下面可以找到 gcc 和 ld 等工具，可以用绝对路径或相对于 ini 文件的路径，不提供的话会搜索 `$PATH` 中的 gcc，比如默认工具链配置可以不写，如果 gcc 刚好在你的 `$PATH` 中的话 |
| gcc |  gcc 工具的可执行文件名，默认不提供的话，就会试图去调用 "gcc"，如果不是这个名字的话，可以用该选项配置一下，比如设置成 "clang" 可以调用 clang，而很多交叉编译工具链前面一般有一串前缀，比如 "arm-linux-androideabi-gcc" 这也，也需要设置 |
| ar | 静态库工具名，同上 |
| ld | 连接器工具名，同上 |
| as | 汇编器工具名，同上 |
| dllwrap | 动态库封装工具名，同上 |
| pkgconfig | pkgconfig 工具名，默认为 `pkg-config` |
| path | 调用工具时额外需要加入 `$PATH` 的路径，用 `,` 分割多个项目；很多工具链的 bin 目录并不会加入系统的 `$PATH`，但里面的工具又会互相调用，使用这个选项可以不污染外部环境变量，只有在 emake 进程里临时设置 `$PATH` 变量，干净一些 |
| cpu | 后面跟一个数字，多核编译的核心数量 |
| target | 目标平台名称，比如 `win32` 和 `linux`，不提供的话会使用 Python 的 `sys.platform` 作为默认值 |
| name | 条件编译的条件变量名称，逗号分割，比如 "android,posix,nossl"，注意 target 的值会自动加入到 name 中，方便根据目标平台进行条件判断  |
| environ | 逗号分隔的环境变量，比如 `environ=FOO=BAR,BARZ=1`，调用工具链前初始化 |
| pcpath | 设置环境变量 `$PKG_CONFIG_PATH` |
| pcflag | 设置 `pkg-config` 的公用参数 |

上面的配置用于确定工具链的位置和运行方式，下面这些用于通用项目配置

| 名称 | 含义 |
|-|-|
| include | 额外的全局 include 目录，逗号分割，类似项目配置里的 `inc` 选项，如果有一些多个项目都需要设置的 include 目录，可以把它设置到工具链配置里，这也就不用每个项目都写了 |
| lib | 额外的全局 lib 目录，逗号分割，类似项目配置里的 `lib` 选项 |
| flag | 全局编译参数，类似项目配置里的 `flag` 选项，编译具体项目时，会添加到项目配置的 flag 前面 |
| cflag| 全局 C 语言编译参数，类似项目配置里的 `cflag` 选项 |
| cxxflag | 全局 C++ 编译参数，类似项目配置里的 `cxxflag` 选项 |
| link | 全局挂载库，比如一些所有项目都要挂载的库 `pthread`, `stdc++` 之类的，写在这里，就不用每个项目都写 |
| define | 全局宏定义 |
| flnk | 连接时传输的参数 |
| wlnk | 连接时使用 `-Wl,` 前缀直接透传给 `ld` 工具的参数 |


#### 配置导入

在工具链配置的 ini 文件里，在非 `[default]` 区可以写一些额外配置：

```ini
[default]
...

[ffmpeg]
include=d:/dev/local/opt/ffmpeg/include
lib=d:/dev/local/opt/ffmpeg/lib
link=avcodec, avdevice, avfilter, avformat, avutil, postproc, swscale

[qt]
include=D:/Dev/Qt/sdk/4.8.3-mingw/include;D:/Dev/Qt/sdk/4.8.3-mingw/include/QtGui
lib=D:/Dev/Qt/sdk/4.8.3-mingw/lib
link=stdc++, ole32, gdi32, wsock32, opengl32, gdi32, glu32, ws2_32, uuid, oleaut32, winmm, imm32, winspool, QtCore4, QtGui4, QtGuid4

```

那么在项目配置里就可以使用 import 来导入：

	import: qt, ffmpeg

那么在你的工程里，上面 qt 和 ffmpeg 的相关配置就会被导入了。

#### 系统包管理

要引入一个包的话，除了用上面的配置导入外，还有一种方式是用 pkg-config，只需要在工程文件里用 `package` 语句即可：

```make
package: python3
```

那么 Emake 会在构建前调用 `pkg-config` 查询 `python3` 这个包的各种编译信息，包括 CFLAGS, LDFLAGS 等，并追加到工程配置中。

当然 pkg-config 依赖 `/usr/lib` 或者 `/usr/local/bin` 下面的 `.pc` 文件，比如：

```text
./lib/x86_64-linux-gnu/pkgconfig/libxcrypt.pc
./lib/x86_64-linux-gnu/pkgconfig/lua.pc
./lib/x86_64-linux-gnu/pkgconfig/formw.pc
./lib/x86_64-linux-gnu/pkgconfig/lua54.pc
./lib/x86_64-linux-gnu/pkgconfig/lua5.4-c++.pc
./lib/x86_64-linux-gnu/pkgconfig/python3.pc
./lib/x86_64-linux-gnu/pkgconfig/ncurses++.pc
./lib/x86_64-linux-gnu/pkgconfig/lua-5.4.pc
```

这些 `.pc` 文件是在安装系统包或者编译第三方包时 `make install` 或者 `cmake --install` 命令生成的，主要用于指定各个包的 `CFLAGS` / `LDFLAGS` 等编译参数以及依赖关系。

而 `pkg-config` 工具，能够加载这些文件处理好依赖并给出正确的 `CFLAGS` / `LDFLAGS` 编译参数。此外，在工程文件里，还可以给出 `pkg-config` 工具调用的额外参数：

```make
pcflag: --atlatest-version
```

这也这个参数就会传递给 `pkg-config`。

还可以在工具链配置文件中，指定 `$PKG_CONFIG_PATH` 这个环境变量：

```ini
[default]
...
pcpath=D:/arm32-devkit/lib/pkgconfig
pcflag=--env-only
...
```

这样调用 `pkg-config` 前会先设置这个环境变量，控制 `pkg-config` 搜索 `.pc` 文件的位置，当然可以配合下一条 `pcflag=--env-only` 跳过默认搜索路径。

通常情况下不用特别设置 `pcpath`，因为工具链自带的 `pkg-config` 一般能够根据自身位置正确搜索到工具链内的 `.pc` 文件的，除非你还有其他位置自己管理额外的 `.pc` 或者工具链不提供 `pkg-config` 工具，你需要依赖一个外部的 `pkg-config` 程序。

这是推荐的包导入方式，而如果一个包没有 `.pc` 文件，`pkg-config` 找不到的话，可以用上面的 `import` 方式导入一个工具链 ini 配置文件里的 section。

## 启动参数

```text
usage: "emake.py <action> [options] srcfile" (emake 3.7.3 Aug.27 2024 win32)

actions  :  -b | -build      build project
            -c | -compile    compile project
            -l | -link       link project
            -r | -rebuild    rebuild project
            -e | -execute    execute project
            -o | -out        show output file name
            -d | -cmdline    call cmdline tool in given environ
            -g | -cygwin     cygwin execute
            -s | -cshell     cygwin shell
            -i | -install    install emake on unix
            -u | -update     update itself from github
            -h | -help       show help page

            -home            display project home
            -list            display project files
            -objs            display obj files
            -cflags          display compile flags
            -depends         display dependencies
            -dirty           display dirty files
            -commands        display compile commands json

options  :  --cfg={cfg}      load config from ~/.config/emake/{cfg}.ini
            --ini={inipath}  load config from {inipath} directly
            --profile={name} set profile to {name}
            --print={n}      set verbose level: 0-3
            --abs={0|1}      display absolute path in error messages

Emake is a easy tool which controls the generation of executables and other
non-source files of a program from the program's source files.
```

## 快速开发

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

## TODO

- [ ] 加入 PIP，支持 pip 一键安装。

