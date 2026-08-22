@echo off
g++ -std=c++17 main.cpp book.cpp library.cpp -o library_system
if %errorlevel% equ 0 (
    echo Build succeeded
) else (
    echo Build failed
)
