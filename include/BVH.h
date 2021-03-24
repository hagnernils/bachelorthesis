//
// Created by Nils Hagner on 23.01.21.
//

#ifndef BACHELORTHESIS_BVH_H
#define BACHELORTHESIS_BVH_H

#include <algorithm>
#include "Aabb.h"
#include "Primitive.h"
#include "gtest/gtest.h"


enum SplitMethod {
    MEDIANCUT = 0,
    MIDPOINT = 1,
};

enum AxisMethod {
    LONGESTAXIS = 0,
    RANDOMAXIS = 1,
};


//template<size_t numChildren>
struct LinearBVHNode {
    Aabb bounds{};
    std::vector<Primitive> primitives;

    LinearBVHNode() = default;
    explicit LinearBVHNode(const Aabb& bounds);

    bool operator==(const LinearBVHNode &rhs) const;

    bool operator!=(const LinearBVHNode &rhs) const;
};

class BVHNode {
public:
    BVHNode() = default;

    bool hit(Ray &ray, Float tMin, Float tMax, HitRecord *hitRecord) const;

    BVHNode(std::vector<std::shared_ptr<Primitive>> &primitives, size_t begin, size_t end);

    void linearize(std::vector<LinearBVHNode> &result, size_t index);

    static void setSampler(std::shared_ptr<DefaultSampler> &sampler);
    static void setConstructionOptions(unsigned int maxPrimsPerLeaf = 50, SplitMethod splitMethod = MEDIANCUT,
                                       AxisMethod axisMethod = LONGESTAXIS);

    friend std::ostream &operator<<(std::ostream &os, const BVHNode &b);

    Aabb bounds;
    size_t depth();
    size_t numNodes();
private:
    FRIEND_TEST(Scene, BuildBVH);
    static AxisMethod axisMethod;
    static SplitMethod splitMethod;
    static unsigned int maxPrimsPerLeaf;
    static std::shared_ptr<DefaultSampler> sampler;
    std::shared_ptr<BVHNode> left = nullptr;
    std::shared_ptr<BVHNode> right = nullptr;
    std::vector<Primitive> primitives{};

    void makeLeaf(const std::vector<std::shared_ptr<Primitive>> &prim,  size_t begin, size_t end);
    bool isLeaf() const { return !primitives.empty(); }
};

#endif //BACHELORTHESIS_BVH_H
