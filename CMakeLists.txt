cmake_minimum_required(VERSION 3.17)
project(bachelorthesis)

set(CMAKE_CXX_STANDARD 20)

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
add_subdirectory(test)

add_executable(bachelorthesis
    main.cpp
    Scene.cpp
    MeshObject.cpp
    Primitive.cpp
    Material.cpp
    Buffer.cpp
    include/Float.h
    include/Material.h
    include/Matrix.h
    include/Primitive.h
    include/Ray.h
    include/Scene.h
    include/Buffer.h
)


target_link_libraries(bachelorthesis libtinygltf)
