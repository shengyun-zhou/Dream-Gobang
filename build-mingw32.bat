@echo off
mkdir build-mingw
cd build-mingw
cmake ..\src -G "MinGW Makefiles"
echo Start building Dream-Gobang...
mingw32-make install
echo If build successfully,the target program file "Dream-Gobang.exe" can be found in build-mingw\bin.
pause