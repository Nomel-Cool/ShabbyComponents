cmake_minimum_required(VERSION 3.10)
project(ShabbyFileManager)

# 设置生成 DLL 的选项
set(BUILD_SHARED_LIBS ON)

# 设置 C++ 标准为 C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 将头文件和源文件添加到项目中
include_directories(${CMAKE_SOURCE_DIR}/Headers)
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/Cpps/*.cpp)

# 添加库目标
add_library(${PROJECT_NAME} ${SOURCES})

# 定义导出宏
target_compile_definitions(${PROJECT_NAME} PRIVATE SHABBY_FILE_MANAGER_EXPORTS)

# 指定生成位置
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/x64
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/x64
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/x64
)

# 添加生成 .lib 文件的目标 
#add_library(${PROJECT_NAME}_static STATIC ${SOURCES}) 
#set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME "${PROJECT_NAME}_static")
