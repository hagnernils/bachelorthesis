cmake_minimum_required(VERSION 3.17)
project(bachelorthesis)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include(GNUInstallDirs)
include_directories(include)
add_subdirectory(test)
add_subdirectory(tinygltf)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})



add_executable(bachelorthesis
    main.cpp
    Scene.cpp
    MeshObject.cpp
    Primitive.cpp
    Material.cpp
    Buffer.cpp
    Estimator.cpp
    BVH.cpp
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
    include/Estimator.h
    include/Spectrum.h
    include/BVH.h
)

# link libraries

target_link_libraries(bachelorthesis libtinygltf)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(bachelorthesis OpenMP::OpenMP_CXX)
endif()