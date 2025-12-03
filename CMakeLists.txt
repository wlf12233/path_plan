cmake_minimum_required(VERSION 3.30)

project(path_plan LANGUAGES CXX VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 设置 C++20 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
#file(GLOB TEST ${PROJECT_SOURCE_DIR}/test/*.cpp)
#主程序库
file(GLOB_RECURSE MAIN_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM MAIN_SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)


#设置项目的可执行文件路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# 让 CMake 先查找 cmake/ 目录下的 CMake 模块
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#把 src 做成一个库，而不是直接加到可执行文件
add_library(path_plan_lib ${MAIN_SOURCES})
target_include_directories(path_plan_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
add_executable(path_plan ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(path_plan PRIVATE path_plan_lib)

enable_testing()
add_subdirectory(test)


