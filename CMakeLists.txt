cmake_minimum_required(VERSION 3.5)

# Set the project name
project (tinyPython)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/Main.cpp
    src/3rd/easylogging++/easylogging++.cpp
    src/BinaryFileParser.cpp
    src/CodeObject.cpp
    src/FileInputStream.cpp
    src/Interpreter.cpp
    src/PyInteger.cpp
    src/PyString.cpp
)

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/3rd/easylogging++
)

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

# Add an executable with the above sources
add_executable(${PROJECT_NAME} ${SOURCES})
