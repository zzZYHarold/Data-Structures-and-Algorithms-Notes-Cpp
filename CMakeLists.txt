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

add_executable(stack_example
        stack/examples/main.cpp
        stack/src/StackApplications.cpp
)

add_executable(array_example
        array/examples/image_flip.cpp
)

target_include_directories(stack_example PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(chapter6_example
        dictionary/examples/chapter6.cpp
        dictionary/src/BidirectionalSearch.cpp
        set/src/UFSets.cpp
        set/src/SetApplications.cpp
)
target_include_directories(chapter6_example PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 第七章：已按 2026 秋最终版课件核对；模板实例化及课件例子检查。
add_executable(chapter7_example
        search/examples/chapter7.cpp
)
target_include_directories(chapter7_example PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 第八章第一组示例：图的两种存储及接口自检（2026 秋最终版课件）。
add_executable(chapter8_part1_example
        graph/examples/chapter8_part1.cpp
)
target_include_directories(chapter8_part1_example PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# 第八章第二部分：遍历、连通分量与最小生成树；Kruskal 依赖并查集实现。
add_executable(chapter8_part2_example
        graph/examples/chapter8_part2.cpp
        set/src/UFSets.cpp
)
target_include_directories(chapter8_part2_example PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(chapter8_part3_example
        graph/examples/chapter8_part3.cpp
)
target_include_directories(chapter8_part3_example PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(chapter9_example
        sorting/examples/chapter9.cpp
)
target_include_directories(chapter9_example PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
