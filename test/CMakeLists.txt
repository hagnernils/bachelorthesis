# subproject Google_Test
project(Google_tests)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 --fast-math")

# 'googletest' is the folder with Google Test sources
add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
add_executable(Google_Tests_run
        MatrixTest.cpp
        PrimitiveTest.cpp
        FloatTest.cpp
        DirectionSamplingTest.cpp
        SceneTest.cpp
        EstimatorTest.cpp
        BVHTest.cpp
        )

target_link_libraries(Google_Tests_run gtest gtest_main)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(Google_Tests_run OpenMP::OpenMP_CXX)
endif()

#target_compile_definitions(Google_Tests_run PRIVATE DOUBLE_PRECISION EXHAUSTIVE_INTERSECTION)