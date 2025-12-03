# 下载 GoogleTest（一次性）
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
FetchContent_MakeAvailable(googletest)

# 查找测试源文件
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(runTests ${TEST_SOURCES})
target_link_libraries(runTests
        PRIVATE
        path_plan_lib
        GTest::gtest
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(runTests)
