# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)
project(MeniscusDistortion VERSION 1.0 LANGUAGES CXX) # Project name
set(CMAKE_CXX_STANDARD 17) # Set C++ standard

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# List your source files manually, excluding undesired files
set(SOURCE_FILES
    Ellipse/Ellipse.h
    Screen/Screen.h
    Ellipse/Ellipse.cpp
    Screen/Screen.cpp
    main.cpp
    main.h
    # Add more source files as needed
)

# Find OpenCV
if (NOT RASPBERRY_PI)
    set(OPEN_CV_BUILD "C:/opencv-4.8.0/build")
    find_package(OpenCV REQUIRED PATHS OPEN_CV_BUILD)
else()
	set(OPEN_CV_BUILD "/usr/local")
    find_package(OpenCV REQUIRED)
endif()

# Link OpenCV libraries
add_executable(${PROJECT_NAME} ${SOURCE_FILES}) # Executable target
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
