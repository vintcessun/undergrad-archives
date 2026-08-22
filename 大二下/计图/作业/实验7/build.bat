@echo off
chcp 65001

if exist CMakeFiles rmdir /s /q CMakeFiles
if exist CMakeCache.txt del /f /q CMakeCache.txt
if exist Makefile del /f /q Makefile
if exist cmake_install.cmake del /f /q cmake_install.cmake

cmake -G "MinGW Makefiles" ^
      -DCMAKE_C_COMPILER="D:/Software/msys64/ucrt64/bin/gcc.exe" ^
      -DCMAKE_CXX_COMPILER="D:/Software/msys64/ucrt64/bin/g++.exe" .

cmake --build .