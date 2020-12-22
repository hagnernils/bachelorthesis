cmake_minimum_required(VERSION 3.17)
project(bachelorthesis)

set(CMAKE_CXX_STANDARD 11)

add_library(libtinygltf INTERFACE)

target_include_directories(libtinygltf INTERFACE
        tinygltf
)

target_sources(libtinygltf INTERFACE
        tinygltf/json.hpp
        tinygltf/stb_image.h
        tinygltf/stb_image_write.h
        tinygltf/tiny_gltf.h
)

include(GNUInstallDirs)
include_directories(include)


add_executable(bachelorthesis
    main.cpp
)


target_link_libraries(bachelorthesis libtinygltf)
