cmake_minimum_required(VERSION 3.10)

project(MyOpenGLProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("D:/Software/msys64/ucrt64/include")
include_directories("D:/Software/msys64/ucrt64/include/eigen3")

link_directories("D:/Software/msys64/ucrt64/lib")

add_executable(exp07_task1 src/exp07_task1.cpp)
add_executable(exp07_task2 src/exp07_task2.cpp)

target_link_libraries(exp07_task1 PRIVATE freeglut opengl32 glu32)
target_link_libraries(exp07_task2 PRIVATE freeglut opengl32 glu32)
