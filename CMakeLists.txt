# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.10)

project ("GUNDAM")

IF(APPLE)
    SET(CMAKE_C_COMPILER "/usr/local/Cellar/gcc/9.1.0/bin/gcc-9")
    SET(CMAKE_CXX_COMPILER "/usr/local/Cellar/gcc/9.1.0/bin/g++-9")
ENDIF()

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)  
  #add_compile_options(/W3 /WX)
  add_compile_options(/wd4819)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

enable_testing()

find_package(GTest REQUIRED)

# 包含子项目。
add_subdirectory ("test")
