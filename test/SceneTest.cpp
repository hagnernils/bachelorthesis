//
// Created by Nils Hagner on 27.12.20.
//

#include "gtest/gtest.h"
#include "../Material.cpp"
#include "../BVH.cpp"
#include "../Scene.cpp"

TEST(Scene, LoadGltf) {
    Scene scene;
    scene.loadGLTF("triangle.gltf");
    auto prims = scene.objects[0].toPrimitives();

    for (auto &prim : prims) {
        std::cerr << prim << std::endl;
    }

    ASSERT_EQ(prims[0]->a,       (Float3{-1,  0,  0}));
    ASSERT_EQ(prims[0]->b,       (Float3{ 1,  0,  0}));
    ASSERT_EQ(prims[0]->c,       (Float3{-1, -2,  0}));
    ASSERT_EQ(prims[0]->normal,  (Float3{ 0,  0, -1}));
}

TEST(Scene, BuildBVH) {
    Scene scene;
    scene.loadGLTF("triangle.gltf");
    scene.buildSceneGeometry();

    std::stringstream ss;
    ss << scene.bvh;
    std::cerr << ss.str();
    ASSERT_EQ(ss.str(), "BVHNode {BVHNode {Leaf with 1 primitives: {}Leaf with 1 primitives: {}}BVHNode {Leaf with 1 primitives: {}Leaf with 1 primitives: {}}}");
}