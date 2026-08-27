cmake_minimum_required(VERSION 4.3)

project(Data_Structures_and_Algorithms_Notes_Cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 自动收集课程代码
file(GLOB_RECURSE COURSE_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

# 排除构建目录
list(FILTER COURSE_FILES EXCLUDE REGEX "/cmake-build-[^/]+/")

# 只编译检查，不生成可执行程序，不需要 main()
add_library(course_code OBJECT
        ${COURSE_FILES}
)

target_include_directories(course_code PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)