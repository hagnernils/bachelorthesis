cmake_minimum_required(VERSION 3.17)
project(tinygltf)

set(CMAKE_CXX_STANDARD 11)

add_library(libtinygltf INTERFACE)

target_include_directories(libtinygltf INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(libtinygltf INTERFACE
        json.hpp
        stb_image.h
        stb_image_write.h
        tiny_gltf.h
)