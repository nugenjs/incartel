cmake_minimum_required(VERSION 3.15)
project(TelemetryConfig)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use vcpkg toolchain if available
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
endif()

# Find required packages
find_package(yaml-cpp CONFIG REQUIRED)

# Create executable for TelemetryConfig
add_executable(TelemetryConfig TelemetryConfig.cpp)

# Link libraries
target_link_libraries(TelemetryConfig PRIVATE yaml-cpp::yaml-cpp)

# Copy YAML config file to build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TelemetryConfig.yaml 
               ${CMAKE_CURRENT_BINARY_DIR}/TelemetryConfig.yaml 
               COPYONLY)

# Set output directory
set_target_properties(TelemetryConfig PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
