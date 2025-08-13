cmake_minimum_required(VERSION 3.10)
project(fb_cube)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
add_executable(fb_cube fb_cube.cpp)
target_link_libraries(fb_cube PRIVATE ${OpenCV_LIBS})

