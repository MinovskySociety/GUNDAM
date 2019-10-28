# CMakeList.txt: TestCMake1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.10)

# TODO: 如有需要，请添加测试并安装目标。

include(GoogleTest)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable (test1 "test1.cc")

#target_link_libraries(test1 PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
target_link_libraries(test1 GTest::GTest GTest::Main)

add_test(AllTests test1)
