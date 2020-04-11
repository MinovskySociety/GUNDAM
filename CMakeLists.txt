cmake_minimum_required (VERSION 3.10)

project ("GUNDAM")

IF(APPLE)
    SET(CMAKE_C_COMPILER "/usr/local/Cellar/gcc/9.1.0/bin/gcc-9")
    SET(CMAKE_CXX_COMPILER "/usr/local/Cellar/gcc/9.1.0/bin/g++-9")
ENDIF()

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

endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

enable_testing()

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

# 包含子项目。
add_subdirectory ("test")
