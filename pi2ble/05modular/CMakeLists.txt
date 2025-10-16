cmake_minimum_required(VERSION 3.15)
project(TelemetrySystem)

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

# Create executable for AppConfig
add_executable(AppConfig AppConfig.cpp)

# Create executable for Main (the main application)
add_executable(Main Main.cpp MockDataCVFrameOut.cpp)

# Add new executables here
# add_executables(Test Test.cpp Something.cpp)

# Link libraries
target_link_libraries(TelemetryConfig PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(AppConfig PRIVATE yaml-cpp::yaml-cpp)
target_link_libraries(Main PRIVATE yaml-cpp::yaml-cpp)

# Copy YAML config files to build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/AppConfig.yaml 
               ${CMAKE_CURRENT_BINARY_DIR}/AppConfig.yaml 
               COPYONLY)

# Set output directory for all targets
set_target_properties(TelemetryConfig AppConfig Main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
