# CMakeList.txt: TestCMake1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.10)

# TODO: 如有需要，请添加测试并安装目标。

include(GoogleTest)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable (test1 "test1.cc")
target_link_libraries(test1 GTest::GTest GTest::Main)
add_test(AllTests test1)

add_executable (test2 "test2.cc")
target_link_libraries(test2 GTest::GTest GTest::Main)
add_test(AllTests test2)

add_executable (test_csvgraph "test_csvgraph.cc")
target_link_libraries(test_csvgraph GTest::GTest GTest::Main)
add_test(AllTests test_csvgraph)

add_executable (test_vf2 "test_vf2.cc")
target_link_libraries(test_vf2 GTest::GTest GTest::Main)
add_test(AllTests test_vf2)


add_executable (test_attribute "test_attribute.cc")
target_link_libraries(test_attribute GTest::GTest GTest::Main)
add_test(AllTests test_attribute)

add_executable (test_multi_vf2 "test_multi_vf2.cc")
target_link_libraries(test_multi_vf2 GTest::GTest GTest::Main)
add_test(AllTests test_multi_vf2)