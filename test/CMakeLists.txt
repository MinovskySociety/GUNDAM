# CMakeList.txt: TestCMake1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.10)

# TODO: 如有需要，请添加测试并安装目标。

include(GoogleTest)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable (test1 "test1.cc")
target_link_libraries(test1 GTest::GTest GTest::Main)
gtest_add_tests(TARGET test1)

add_executable (test2 "test2.cc")
target_link_libraries(test2 GTest::GTest GTest::Main)
#gtest_add_tests(TARGET test2)

add_executable (test_csvgraph "test_csvgraph.cc")
target_link_libraries(test_csvgraph GTest::GTest GTest::Main)
#gtest_add_tests(TARGET test_csvgraph)

add_executable (test_vf2 "test_vf2.cc")
target_link_libraries(test_vf2 GTest::GTest GTest::Main)
#gtest_add_tests(TARGET test_vf2)

add_executable (test_multi_vf2 "test_multi_vf2.cc")
target_link_libraries(test_multi_vf2 GTest::GTest GTest::Main)
#gtest_add_tests(TARGET test_multi_vf2)

add_executable (test_idgen "test_idgen.cc")
target_link_libraries(test_idgen GTest::GTest GTest::Main)
gtest_add_tests(TARGET test_idgen)

add_executable (test_matchresult "test_writematchresult.cc")
target_link_libraries(test_matchresult GTest::GTest GTest::Main)
#gtest_add_tests(TARGET test_matchresult)

add_executable (test_datatype "test_datatype.cc")
target_link_libraries(test_datatype GTest::GTest GTest::Main)
#gtest_add_tests(TARGET test_datatype)

#add_executable (test_container "test_container.cc")
#target_link_libraries(test_container GTest::GTest GTest::Main)
#add_test(AllTests test_container)

#add_executable (test_iterator "test_iterator.cc")
#target_link_libraries(test_iterator GTest::GTest GTest::Main)
#add_test(AllTests test_iterator)

#add_executable (test_attribute "test_attribute.cc")
#target_link_libraries(test_attribute GTest::GTest GTest::Main)
#add_test(AllTests test_attribute)

add_executable (test_graph "test_graph.cc")
target_link_libraries(test_graph GTest::GTest GTest::Main)
#gtest_add_tests(TARGET test_graph)

add_executable (test_simple_graph "test_simple_graph.cc")
target_link_libraries(test_simple_graph GTest::GTest GTest::Main)
#gtest_add_tests(TARGET test_simple_graph)

add_executable (test_iterator2 "test_iterator2.cc")
target_link_libraries(test_iterator2 GTest::GTest GTest::Main)
#gtest_add_tests(TARGET test_iterator2)

add_executable (test_container2 "test_container2.cc")
target_link_libraries(test_container2 GTest::GTest GTest::Main)
gtest_add_tests(TARGET test_container2)

add_executable (test_pointer "test_pointer.cc")
target_link_libraries(test_pointer GTest::GTest GTest::Main)
#gtest_add_tests(TARGET test_pointer)