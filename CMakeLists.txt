cmake_minimum_required(VERSION 3.2)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

project(CS6610)
add_executable(CS6610 ./main.cpp)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(./ExternalLibraries/cyCodeBase/)
include_directories(${GLFW_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIR})

target_link_libraries(CS6610 glfw)
target_link_libraries(CS6610 ${OPENGL_LIBRARIES})

add_custom_command(
        TARGET CS6610 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/FragShader.glsl
                ${CMAKE_CURRENT_BINARY_DIR}/FragShader.glsl
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/FragShader.glsl
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/VertexShader.glsl
                ${CMAKE_CURRENT_BINARY_DIR}/VertexShader.glsl
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/VertexShader.glsl
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/teapot.obj
                ${CMAKE_CURRENT_BINARY_DIR}/teapot.obj
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/teapot.obj)
