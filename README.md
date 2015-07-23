# Dream Gobang（梦幻五子棋）
![game-logo](https://raw.githubusercontent.com/BUPTSSE-Zero/Dream-Gobang/master/src/res/game-logo.png "Game-Logo")

## 简介
`Dream Gobang`（梦幻五子棋）是一款基于WindowsGDI引擎的简易五子棋游戏，目前该游戏可以进行单机人机对战。

## 系统平台要求
Microsoft Windows XP及以上

## 开发者
Dream-Gobang Team 五人小组

## 使用的第三方库
[EGE(Easy Graphics Engine)](https://github.com/misakamm/xege)

[libpng](http://www.libpng.org/)(Referred by EGE)

## 许可证
![gplv3](http://www.gnu.org/graphics/gplv3-127x51.png "GPL-V3")

## 构建方法
### （1）安装 [CMake](http://www.cmake.org/download) (Win32 Installer)

### （2）打开CMake-GUI程序

![](https://raw.githubusercontent.com/BUPTSSE-Zero/Dream-Gobang/master/tutorial-picture/cmake-gui.png "CMake-GUI")

①选择`Dream Gobang`源码所在目录。

②选择用于构建`Dream Gobang`的目录（也就是要生成的工程文件的目录）。（该目录以下称作`%BUILD%`）

③点击“Generate”按钮，根据所用编译器生成相应的工程文件。
（注意：如果打算使用MinGW编译器构建，请先将MinGW编译器所在的路径添加到系统环境的`PATH`变量中）

### （3）开始构建
#### 使用Visual Studio构建

生成工程文件后，进入`%BUILD%`目录，打开`Dream-Gobang.sln`解决方案文件。

![](https://raw.githubusercontent.com/BUPTSSE-Zero/Dream-Gobang/master/tutorial-picture/vs2013-enu.PNG "vs2013")

①根据需要选择生成方案。

②单独构建`INSTALL`项目。

构建完成后，生成的`Dream-Gobang.exe`可执行文件位于`%BUILD%\bin`目录中对应的生成方案目录里。

#### 使用MinGW构建
生成`Makefile`之后，在`%BUILD%`目录下打开控制台，输入如下命令即可构建:

```
mingw32-make install
```
构建完成后，生成的`Dream-Gobang.exe`可执行文件位于`%BUILD%\bin`目录里.

## To Do
1.优化界面效果及用户交互效果。

2.加入联机对战。

3.优化人机对战算法。
