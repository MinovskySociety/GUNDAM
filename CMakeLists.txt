<<<<<<< HEAD
﻿# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.10)
=======
﻿cmake_minimum_required (VERSION 3.10)
>>>>>>> b47e3df9b55c5c979efa4b87485ec6f7c7a6ce6c

project ("GUNDAM")

IF(APPLE)
    SET(CMAKE_C_COMPILER "/usr/local/Cellar/gcc/9.1.0/bin/gcc-9")
    SET(CMAKE_CXX_COMPILER "/usr/local/Cellar/gcc/9.1.0/bin/g++-9")
ENDIF()

<<<<<<< HEAD
if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)  
  add_compile_options(/W3 /WX)
  add_compile_options(/wd4819)
=======
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  message("MSVC")

  add_definitions(-D_CRT_SECURE_NO_WARNINGS)  
  add_compile_options(/WX /wd4819)
  
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  message("GNU")
  #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g  -m64 -fexpensive-optimizations -O3" )

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  message("Clang")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  message("Intel")

>>>>>>> b47e3df9b55c5c979efa4b87485ec6f7c7a6ce6c
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

enable_testing()

<<<<<<< HEAD
find_package(GTest REQUIRED)

=======
#gtest
find_package(GTest REQUIRED)

#openmp
find_package(OpenMP)
if(OPENMP_FOUND)
  message("OpenMP FOUND")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

>>>>>>> b47e3df9b55c5c979efa4b87485ec6f7c7a6ce6c
# 包含子项目。
add_subdirectory ("test")
