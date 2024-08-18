cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Relive)
find_package(Vulkan REQUIRED)
find_package(glfw3 3.4 REQUIRED)
add_executable(Relive ./src/main.cpp)

target_include_directories(Relive
    PUBLIC ${Vulkan_INCLUDE_DIRS})

target_link_libraries(Relive 
    ${Vulkan_LIBRARIES}
    glfw)

