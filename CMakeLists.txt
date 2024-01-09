# Minimum CMake version required

cmake_minimum_required(VERSION 3.10)
project(MeniscusDistortion VERSION 1.0 LANGUAGES CXX) # Project name
set(CMAKE_CXX_STANDARD 17) # Set C++ standard

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# List your source files manually, excluding undesired files
set(SOURCE_FILES
    main.h
    include/Ellipse.h
    include/MQTT++.h
    include/Screen.h
    
    main.cpp
    src/Ellipse.cpp
    src/Screen.cpp
)

include_directories("/usr/local/include")
# Find OpenCV
if (NOT RASPBERRY_PI)
    set(OPEN_CV_BUILD "C:/opencv-4.8.0/build")
    find_package(OpenCV REQUIRED PATHS ${OPEN_CV_BUILD})
else()
    set(OPEN_CV_BUILD "/usr/local")
    find_package(OpenCV REQUIRED)
endif()

# Find Paho MQTT C++
find_package(PahoMqttCpp REQUIRED)
find_library(PAHO_MQTT_C_LIB paho-mqtt3c REQUIRED)
find_library(PAHO_MQTT_AS_LIB paho-mqtt3as REQUIRED)

if(PahoMqttCpp_FOUND)
	message(STATUS "Found PahoMqttCpp package")
endif()
if(PAHO_MQTT_C_LIB)
	message(STATUS "Found paho-mqtt3c")
endif()
if(PAHO_MQTT_AS_LIB)
	message(STATUS "Found paho-mqtt3as")
endif()

message(STATUS ${PAHO_MQTT_C_LIB})
message(STATUS ${PAHO_MQTT_AS_LIB})

# Link OpenCV and Paho MQTT libraries
add_executable(${PROJECT_NAME} ${SOURCE_FILES}) # Executable target
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} paho-mqttpp3 paho-mqtt3c)
