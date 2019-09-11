# 头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)
# 源文件列表
aux_source_directory(. SRCS)
# 可执行文件目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
# 可执行文件
add_executable(gundam ${SRCS})
