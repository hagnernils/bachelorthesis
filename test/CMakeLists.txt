# subproject Google_Test
project(Google_tests)

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
        EstimatorTest.cpp)

target_link_libraries(Google_Tests_run gtest gtest_main)