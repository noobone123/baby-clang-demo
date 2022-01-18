# 指定 cmake 的最小版本
cmake_minimum_required(VERSION 3.4.3)

# 定义工程的名称
project(BabyCodeStyleChecker)

# Add the location of ClangConfig.cmake to CMake search paths (so that
# find_package can locate it)
list(APPEND CMAKE_PREFIX_PATH "${CT_Clang_INSTALL_DIR}/lib/cmake/clang/")

find_package(Clang REQUIRED CONFIG)

# 设置头文件的包含路径
# plugins includes headers from LLVM and Clang - update the include paths accordingly
include_directories(SYSTEM "${LLVM_INCLUDE_DIRS};${CLANG_INCLUDE_DIRS}")

# LLVM is normally built without RTTI. Be consistent with that.
if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

# ======================================================
# 设置插件名
set(MY_PLUGINS
      CodeStyleChecker
    )

# 设置代码文件名
set(CodeStyleChecker_SOURCES
  myCodeStyleChecker_plugin.cpp)

# 设置工具名
set(My_Clang_TOOLS
    ct_CodeStyleChecker
)

# 设置代码文件名
set(ct_CodeStyleChecker_SOURCES
  myCodeStyleChecker_tool.cpp
)

# Configure the plugins
# ==============================
foreach( plugin ${MY_PLUGINS} )
    # 编译静态库 add_library(库名称 STATIC 代码文件名称)
    # 编译动态库 add_library(库名称 SHARED 代码文件名称)
    # Create a library corresponding to 'plugin'
    add_library(
      ${plugin}
      SHARED
      ${${plugin}_SOURCES}
      )

    # Configure include directories for 'plugin'
    # 指定目标包含的头文件路径
    target_include_directories(
      ${plugin}
      PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )

    # 指定目标链接的库
    # target_link_libraries(所需生成的文件名称 所需链接的动态库名称)
    target_link_libraries(
      ${plugin}
    )
endforeach()


# CONFIGURE THE TOOLS
# ===================
foreach( tool ${My_Clang_TOOLS} )
    # Create a library corresponding to 'plugin'
    add_executable(
      ${tool}
      ${${tool}_SOURCES}
      )

    # Configure include directories for 'tool'
    target_include_directories(
      ${tool}
      PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/.."
    )

    # Link with libclangTooling
    target_link_libraries(
      ${tool}
      clangBasic
      clangFrontend
      clangSerialization
      clangTooling
    )
endforeach()