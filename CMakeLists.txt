# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)
project(MeniscusDistortion VERSION 1.0 LANGUAGES CXX) # Project name
set(CMAKE_CXX_STANDARD 17) # Set C++ standard

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# List your source files manually, excluding undesired files
set(SOURCE_FILES
    Ellipse/Ellipse.cpp
    Ellipse/Ellipse.h
    main.cpp
    main.h
    # Add more source files as needed
)

# Find OpenCV
set(OPEN_CV_BUILD "C:/opencv-4.8.0/build")
find_package(OpenCV REQUIRED PATHS OPEN_CV_BUILD)

# Link OpenCV libraries
add_executable(${PROJECT_NAME} ${SOURCE_FILES}) # Executable target
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
