//
// Created by Nils Hagner on 17.01.21.
//

#include "gtest/gtest.h"
#include "BVH.h"

TEST(BVH, BvhConstruction) {
    BVHNode bvh;
    auto sampler = std::make_shared<DefaultSampler>();
    sampler->seed(12345);
    bvh.setSampler(sampler);
    bvh.setConstructionOptions(1);
    std::vector<std::shared_ptr<Primitive>> primitives;

    // construct prims like this:
    //   prim2 prim3     (z = 1)
    //   prim0 prim1     (z = 0)
    for (auto i = 0; i < 1; i++) {
        Float z = i;
        Float3 a0 = {0, 0, z}, b0 = {1, 0, z}, c0 = {0, 1, z};
        Float3 a1 = {1, 0, z}, b1 = {2, 0, z}, c1 = {1, 1, z};
        Float3 a2 = {0, 2, z}, b2 = {1, 2, z}, c2 = {0, 3, z};
        Float3 a3 = {1, 2, z}, b3 = {2, 2, z}, c3 = {1, 3, z};
        primitives.emplace_back(std::make_shared<Primitive>(a0, b0, c0, nullptr));
        primitives.emplace_back(std::make_shared<Primitive>(a1, b1, c1, nullptr));
        primitives.emplace_back(std::make_shared<Primitive>(a2, b2, c2, nullptr));
        primitives.emplace_back(std::make_shared<Primitive>(a3, b3, c3, nullptr));
    }
    bvh = BVHNode(primitives, 0, primitives.size());

    std::stringstream ss;
    ss << bvh;
    std::cerr << ss.str();
    ASSERT_EQ(ss.str(), "BVHNode {BVHNode {Leaf (1) Leaf (1) }BVHNode {Leaf (1) Leaf (1) }}");

    std::vector<LinearBVHNode> linearBvh(2 << (bvh.depth() + 1));
    bvh.linearize(linearBvh, 0);

    ASSERT_EQ(linearBvh[0], LinearBVHNode(Aabb{{0,0,0}, {2,3,0}}));
}