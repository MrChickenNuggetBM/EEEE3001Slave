# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(MeniscusDistortion)

# Find OpenCV
find_package(OpenCV REQUIRED PATHS "C:/opencv-4.8.0/build")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# List your source files manually, excluding undesired files
set(SOURCE_FILES
    Ellipse/Ellipse.cpp
    Ellipse/Ellipse.h
	main.cpp
	main.h
    # Add more source files as needed
)

# Executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link OpenCV libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
