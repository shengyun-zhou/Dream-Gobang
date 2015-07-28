# AppVeyor MinGW/MSYS build script

mount C:\MinGW /mingw
export PATH=/usr/local/bin:/mingw/bin:/bin:/c/mingw/bin:$PATH
cd /c/projects/Dream-Gobang/src

mkdir build-mingw
cd build-mingw

echo Configuring for MinGW/MSYS ...
cmake .. -G "MSYS Makefiles"

echo Building for MinGW/MSYS ...
make install
