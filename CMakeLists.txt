cmake_minimum_required(VERSION 3.5)

# 设置项目的名称
project (tinyPython)

# 启动gdb
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")


# 检查c++编译器标志，设置c++11支持变量
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

# 使用变量设置编译标志
if(COMPILER_SUPPORTS_CXX11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# 创建一个源文件变量以用于链接所有的cpp文件用于编译
set(SOURCES
    src/Main.cpp
    src/3rd/easylogging++/easylogging++.cpp
    src/PycFileParser.cpp
    src/CodeObject.cpp
    src/FileInputStream.cpp
    src/Interpreter.cpp
    src/PyObject.cpp
    src/PyInteger.cpp
    src/PyDouble.cpp
    src/PyString.cpp
    src/PyList.cpp
    src/PyDict.cpp
    src/Klass.cpp
    src/VM.cpp
    src/LoggerConfig.cpp
    src/Frame.cpp
    src/PyFunction.cpp
    src/Native.cpp
    src/ArrayList.cpp
    src/Map.cpp
    src/PyListIterator.cpp
    src/PyStringIterator.cpp
    src/PyDictIterator.cpp
    src/PyTuple.cpp
)

# 添加一个可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 添加头文件目录
# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/object
        ${PROJECT_SOURCE_DIR}/src/os
        ${PROJECT_SOURCE_DIR}/src/cpu
        ${PROJECT_SOURCE_DIR}/src/3rd/easylogging++
)
