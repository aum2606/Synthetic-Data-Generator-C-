cmake_minimum_required(VERSION 3.10)
project(SyntheticDataGenerator VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# We're now using our own image implementation, so we don't need OpenCV
add_definitions(-DUSE_OPENCV)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    src/Synthetic\ Data\ Generator.cpp
    src/data_types/TabularData.cpp
    src/data_types/ImageData.cpp
    src/data_types/TextData.cpp
    src/data_types/TimeSeriesData.cpp
    src/data_types/AudioData.cpp
    src/utils/RandomGenerators.cpp
    src/utils/Distributions.cpp
    src/utils/FileExport.cpp
)

# Add executable
add_executable(SyntheticDataGenerator ${SOURCES})

# Create output directory
set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/output)
file(MAKE_DIRECTORY ${OUTPUT_DIR})
