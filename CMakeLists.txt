cmake_minimum_required(VERSION 4.3)

project(Data_Structures_and_Algorithms_Notes_Cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ============================================================
# 1. 课程代码：自动收集并统一编译检查
#
# 大部分课程代码只有数据结构 / 算法实现，没有 main()，
# 因此使用 OBJECT library：
#   - 会实际编译各个 .cpp
#   - 可以检查编译错误
#   - 不进行最终链接
#   - 不要求存在 main()
# ============================================================

file(GLOB_RECURSE COURSE_FILES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

# 排除 CMake 构建目录
list(FILTER COURSE_FILES EXCLUDE REGEX
        "/cmake-build-[^/]+/"
)

# examples 中的 .cpp 是可执行示例，
# 不放进 course_code，由各自的 executable target 管理
list(FILTER COURSE_FILES EXCLUDE REGEX
        "/examples/.*\\.cpp$"
)

add_library(course_code OBJECT
        ${COURSE_FILES}
)

# 以项目根目录作为头文件搜索起点
#
# 因此可以写：
# #include "stack/include/SeqStack.h"
# #include "linked_list/..."
target_include_directories(course_code PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)


# ============================================================
# 2. Chapter 3 - Stack Example
# ============================================================

add_executable(stack_example
        stack/examples/main.cpp
        stack/src/StackApplications.cpp
)

target_include_directories(stack_example PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)