//
// Created by Nils Hagner on 23.01.21.
//

#ifndef BACHELORTHESIS_BVH_H
#define BACHELORTHESIS_BVH_H

#include <algorithm>
#include "Aabb.h"
#include "Primitive.h"


enum SplitMethod {
    MEDIANCUT,
    MIDPOINT
};

enum AxisMethod {
    LONGESTAXIS,
    RANDOMAXIS
};


//template<size_t numChildren>
struct LinearBVHNode {
    Aabb bounds{};
    std::vector<Primitive> primitives;

    LinearBVHNode() = default;
    explicit LinearBVHNode(Aabb bounds);

    bool operator==(const LinearBVHNode &rhs) const;

    bool operator!=(const LinearBVHNode &rhs) const;
};

class BVHNode {
public:
    BVHNode() = default;

    bool hit(Ray &ray, Float tMin, Float tMax, HitRecord *hitRecord) const;

    BVHNode(std::vector<std::shared_ptr<Primitive>> &primitives, size_t begin, size_t end, unsigned int maxPrimsPerLeaf=50);

    void linearize(std::vector<LinearBVHNode> &result, size_t index);

    void setSampler(std::shared_ptr<DefaultSampler> &sampler);

    friend std::ostream &operator<<(std::ostream &os, const BVHNode &b);

    Aabb bounds;
    size_t depth();
    size_t numNodes();
private:
    static std::shared_ptr<DefaultSampler> sampler;
    std::shared_ptr<BVHNode> left = nullptr;
    std::shared_ptr<BVHNode> right = nullptr;
    std::vector<Primitive> primitives{};
    AxisMethod axisMethod = LONGESTAXIS;
    SplitMethod splitMethod = MEDIANCUT;

    void makeLeaf(const std::vector<std::shared_ptr<Primitive>> &prim,  size_t begin, size_t end);
    bool isLeaf() const { return !primitives.empty(); }
};

#endif //BACHELORTHESIS_BVH_H
