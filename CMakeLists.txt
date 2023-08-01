
cmake_minimum_required(VERSION 3.12)

project(OGLAPP)

add_executable(OGLAPP  main.cpp "dep/objLoader.cpp"  "dep/CamContr.cpp" "dep/shader.cpp")

target_include_directories(OGLAPP PRIVATE libs/glm 
                                              libs/glfw/include
                                              libs/glew/include
                                              libs/stb)

target_link_directories(OGLAPP PRIVATE libs/glfw/lib-vc2022
                                           libs/glew/lib/Release/x64)

target_link_libraries(OGLAPP PRIVATE glfw3.lib glew32.lib opengl32.lib)

add_custom_command(TARGET OGLAPP POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/libs/glew/bin/Release/x64/glew32.dll" "${CMAKE_BINARY_DIR}/glew32.dll")