cmake_minimum_required(VERSION 3.30)

project(path_plan VERSION 1.0)

# 设置 C++20 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#设置项目的可执行文件路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# 让 CMake 先查找 cmake/ 目录下的 CMake 模块
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# 设置头文件目录
#target_include_directories(path_plan PRIVATE include)
#include_directories(${PROJECT_SOURCE_DIR}/include)

#添加子目录
add_subdirectory(src)


