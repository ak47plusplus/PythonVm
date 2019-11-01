cmake_minimum_required(VERSION 3.5)

# Set the project name
project (tinyPython)

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/ArrayList.cpp
    src/BinaryFileParser.cpp
    src/CodeObject.cpp
    src/FileInputStream.cpp
    src/Interpreter.cpp
    src/PyInteger.cpp
    src/PyString.cpp
    src/Main.cpp
)

# Add an executable with the above sources
add_executable(${PROJECT_NAME} ${SOURCES})


# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/3rd/easylogging++
)
