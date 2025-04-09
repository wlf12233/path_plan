file(GLOB SOURCES *.cpp)

add_executable(path_plan ${SOURCES})

# 链接库
target_include_directories(path_plan PUBLIC ${PROJECT_SOURCE_DIR}/include)