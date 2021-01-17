cmake_minimum_required(VERSION 3.17)
project(bachelorthesis)

set(CMAKE_CXX_STANDARD 20)

include(GNUInstallDirs)
include_directories(include)
add_subdirectory(test)
add_subdirectory(tinygltf)

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
    include/Aabb.h
    include/DirectionSampling.h
    include/Sampler.h
    include/HitRecord.h
)


target_link_libraries(bachelorthesis libtinygltf)
