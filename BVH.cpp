//
// Created by Nils Hagner on 16.02.21.
//
#include "BVH.h"
#include <utility>

std::shared_ptr<DefaultSampler> BVHNode::sampler;

void BVHNode::setSampler(std::shared_ptr <DefaultSampler> &sampler) { BVHNode::sampler = sampler; }

void BVHNode::linearize(std::vector<LinearBVHNode> &result, size_t index) {
    result[index] = LinearBVHNode(bounds);
    if (isLeaf()) {
        result[index].primitives = primitives;
        return;
    }
    left->linearize(result, 2 * index + 1);
    right->linearize(result, 2 * index + 2);
}

BVHNode::BVHNode(std::vector<std::shared_ptr<Primitive>> &primitives, size_t begin, size_t end, int maxPrimsPerLeaf) {
    auto treeSize = end - begin;

    for (auto i = begin; i < end; i++)
        bounds += (primitives[i]->bounds);

    if (treeSize <= maxPrimsPerLeaf) {
        makeLeaf(primitives, begin, end);
        return;
    } else {
        std::vector<Float3> centers;
        Aabb centroidBounds{};
        size_t splitAxis;
        Float splitValue;
        size_t secondSetBegin;

        for (auto i = begin; i < end; i++) {
            auto c = primitives[i]->bounds.center();
            centers.push_back(c);
            centroidBounds += c;
        }
        if (axisMethod == LONGESTAXIS) {
            splitAxis = centroidBounds.longestAxis();
        } else if (axisMethod == RANDOMAXIS) {
            splitAxis = sampler->uniformInt(0, 2);
        }

        if (splitMethod == MIDPOINT) {
            splitValue = ((centroidBounds.min + centroidBounds.max) / 2.f)()[splitAxis];
            auto partitioner = [splitAxis, splitValue](std::shared_ptr<Primitive> &p){ return p->bounds.center()()[splitAxis] < splitValue; };
            auto partition = std::partition(primitives.begin() + begin, primitives.begin() + end, partitioner);
            secondSetBegin = partition - primitives.begin();
        } else if (splitMethod == MEDIANCUT) {
            auto comparator = [splitAxis](std::shared_ptr<Primitive> &p1,
                                          std::shared_ptr<Primitive> &p2){ return p1->bounds.center()()[splitAxis] < p1->bounds.center()()[splitAxis]; };
            std::sort(primitives.begin(), primitives.end(), comparator);
            secondSetBegin = begin + treeSize / 2;
        }

        left = std::make_shared<BVHNode>(primitives, begin, secondSetBegin, maxPrimsPerLeaf);
        right = std::make_shared<BVHNode>(primitives, secondSetBegin, end, maxPrimsPerLeaf);
    }
}

bool BVHNode::hit(Ray &ray, Float tMin, Float tMax, HitRecord *hitRecord) const {
    if (!bounds.hit(ray, tMin, tMax, hitRecord))
        return false;

    if (isLeaf()) {
        bool hit = false;
        for (auto &p: primitives) {
            if (p.hit(ray, tMin, tMax, hitRecord)) {
                tMax = hitRecord->time;
                hit = true;
            }
        }
        return hit;
    }
    bool hitLeft = left->hit(ray, tMin, tMax, hitRecord);
    Float dist = hitLeft ? hitRecord->time : tMax;
    bool hitRight = right->hit(ray, tMin, dist, hitRecord);
    return hitLeft || hitRight;
}

void BVHNode::makeLeaf(const std::vector<std::shared_ptr<Primitive>> &prims, size_t begin, size_t end) {
    for (auto i = begin; i < end; i++)
        primitives.push_back(*prims[i]);
    left = nullptr;
    right = nullptr;
}

std::ostream &operator<<(std::ostream &os, const BVHNode &b) {
    if (!b.primitives.empty()) {
        os << "Leaf with " << b.primitives.size() << " primitives: {";
        //for (auto p: b.primitives)
        //    os << p << " ";
        return os << "}";
    } else {
        os << "BVHNode {";
        os << *b.left;
        os << *b.right;
        return os << "}";
    }
}

size_t BVHNode::depth() {
    return std::max((left != nullptr) ? left->depth() + 1 : 0, (right != nullptr) ? right->depth() + 1 : 0);
}

size_t BVHNode::numNodes() {
    return (left != nullptr ? left->numNodes() + 1 : 0 ) + (right != nullptr ? right->numNodes() + 1 : 0);
}

LinearBVHNode::LinearBVHNode(Aabb bounds) : bounds(std::move(bounds)) {}

bool LinearBVHNode::operator==(const LinearBVHNode &rhs) const {
    return bounds == rhs.bounds &&
           primitives == rhs.primitives;
}

bool LinearBVHNode::operator!=(const LinearBVHNode &rhs) const {
    return !(rhs == *this);
}

/*
// because the nodes are used in a min-heap, but C++ provides a max-heap, we return the opposite
bool operator<(const Node &other) const {
    return estimatedDistance > other.estimatedDistance;
}

// Kay and Kajiyas heap-based algorithm from rendering of complex scemes
bool hit(Ray &ray, Float tMin, Float tMax, HitRecord *hitRecord) {
    // the min heap storing all current candidates
    std::vector<BVHNode> heap = {root};

    while (!heap.empty()) {
        // get closest candidate box
        std::pop_heap(heap.begin(), heap.end());
        auto candidate = heap.back();
        if (candidate.isLeaf())
            return candidate.primitive->hit(ray, tMin, tMax, hitRecord);
        for (const auto& child : candidate.children) {
            if (child.bounds.hit(ray, tMin, tMax, hitRecord)) {
                heap.push_back(child);
                std::push_heap(heap.begin(), heap.end());
            }
        }
    }
}
*/