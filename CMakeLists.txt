cmake_minimum_required(VERSION 3.2)

project(CS6610)
add_executable(CS6610 ./main.cpp)

# find_package(PkgConfig REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

# pkg_search_module(GLFW REQUIRED glfw3)

include_directories(${GLFW_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})

target_link_libraries(CS6610 glfw)
target_link_libraries(CS6610 ${OPENGL_LIBRARIES})
