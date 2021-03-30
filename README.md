## Monte Carlo Raytracing for calculating of diffuse-specular radiation exchange factors 

This project is part of my bachelor thesis and aims to use Monte Carlo Raytracing techniques for REF computation.

Using C++, CMake, and other cool stuff.

### libraries used
- [Google Test](https://github.com/google/googletest)
- [tinygltf](https://github.com/syoyo/tinygltf)


### algorithms implemented
- Müller-Trumbore triangle intersection (Primitive.cpp)
- A BVH using a top-down median or midpoint split (BVH.cpp)
- cosine weighted and uniform hemisphere sampling (DirectionSampling.h)
- uniform surface sampling on triangles (Primitive.cpp)
- own vector and matrix library (Float.h, Matrix.h, Matrix.cpp)
- AABBs and their intersection (Aabb.h)
- A MC estimator for REFs (Estimator.cpp)

### program arguments
The positional arguments for the executable are read in main.cpp and include

- maximum number of primitves per leaf (lower faster)
- surface samples per triangle. Direction samples are set to 1.
- maximum trace depth
- useMedianSplit: whether to use the median (true) or midpoint (false) split
- useUniformSampling: to use uniform or cosine-weighted hemisphere sampling
- sceneName: the filename of the gltf file without ending
- outfilename (optional): where to write the result to. Output is also given on the command line.
