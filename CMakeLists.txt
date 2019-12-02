cmake_minimum_required(VERSION 3.5)

# 设置项目的名称
project (tinyPython)

set(CMAKE_CXX_STANDARD 11)

# 启动gdb
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# boost
#set(Boost_USE_STATIC_LIBS ON)
set(BOOST_ROOT "C:\\Users\\mzygd\\Desktop\\boost_1_70_0")
#set(BOOST_LIBRARYDIR "C:\\Users\\mzygd\\Desktop\\boost_1_70_0\\stage\\lib")
find_package(Boost COMPONENTS filesystem REQUIRED)

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
    src/Klass.cpp
    src/VM.cpp
    src/LoggerConfig.cpp
    src/Frame.cpp
    src/Native.cpp
    src/ArrayList.cpp
    src/Map.cpp
    src/object/PyObject.cpp
    src/object/PyInteger.cpp
    src/object/PyDouble.cpp
    src/object/PyString.cpp
    src/object/PyStringIterator.cpp
    src/object/PyFunction.cpp
    src/object/PyList.cpp
    src/object/PyListIterator.cpp
    src/object/PyDict.cpp
    src/object/PyDictIterator.cpp
    src/object/PyTuple.cpp
    src/object/PySlice.cpp
    src/object/PyTypeObject.cpp)

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

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
    #include_directories(${Boost_INCLUDE_DIR})
    #link_directories(${Boost_LIBRARY_DIR})
endif()
