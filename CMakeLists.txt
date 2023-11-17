# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(MeniscusDistortion)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add a variable for source files directory
set(SOURCE_DIR Ellipse)

# Collect source files (both .cpp and .h) from the specified directory
file(GLOB_RECURSE SOURCE_FILES ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.h)

# Executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
