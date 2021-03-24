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
    {
        Scene scene;
        scene.loadGLTF("sceneTriangleTest.gltf");
        BVHNode::setConstructionOptions(1);
        scene.buildSceneGeometry();
    }
    {
        Scene scene;
        scene.loadGLTF("bvhtest.gltf");
        BVHNode::setConstructionOptions(2, SplitMethod::MEDIANCUT, AxisMethod::LONGESTAXIS);
        scene.buildSceneGeometry();

        std::stringstream ss;
        ss << scene.bvh;
        ASSERT_EQ(ss.str(), "BVHNode {"
                            "BVHNode {"
                                "BVHNode {"
                                    "BVHNode {Leaf (2) Leaf (2) }"
                                    "BVHNode {Leaf (2) Leaf (2) }"
                                "}"
                                "BVHNode {"
                                    "BVHNode {Leaf (2) Leaf (2) }"
                                    "BVHNode {Leaf (2) Leaf (2) }"
                                "}"
                            "}"
                            "BVHNode {"
                                "BVHNode {"
                                    "BVHNode {Leaf (2) Leaf (2) }"
                                    "BVHNode {Leaf (2) Leaf (2) }"
                                "}"
                                "BVHNode {"
                                    "BVHNode {Leaf (2) Leaf (2) }"
                                    "BVHNode {Leaf (2) Leaf (2) }"
                                "}"
                            "}"
                        "}");
    }
    {
        Scene scene1;
        scene1.loadGLTF("bvhtest2.gltf");
        BVHNode::setConstructionOptions(1);
        scene1.buildSceneGeometry();
        std::stringstream ss;
        ss << scene1.bvh;
        ASSERT_EQ(ss.str(), "BVHNode {"
                                "BVHNode {"
                                    "Leaf (1) "
                                    "Leaf (1) }"
                                "BVHNode {"
                                    "Leaf (1) "
                                    "Leaf (1) }"
                            "}");

        // left - left and left-right leaves
        ASSERT_EQ(scene1.bvh.left->left->primitives[0].parent->objectID, 0);
        ASSERT_EQ(scene1.bvh.left->left->primitives[0].parent->name, "A");

        ASSERT_EQ(scene1.bvh.left->right->primitives[0].parent->objectID, 2);
        ASSERT_EQ(scene1.bvh.left->right->primitives[0].parent->name, "D");

        // right-left and right-right leaves
        ASSERT_EQ(scene1.bvh.right->left->primitives[0].parent->objectID, 1);
        ASSERT_EQ(scene1.bvh.right->left->primitives[0].parent->name, "C");

        ASSERT_EQ(scene1.bvh.right->right->primitives[0].parent->objectID, 3);
        ASSERT_EQ(scene1.bvh.right->right->primitives[0].parent->name, "B");
    }
}