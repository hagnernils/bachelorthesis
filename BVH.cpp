//
// Created by Nils Hagner on 16.02.21.
//
#include "BVH.h"

std::shared_ptr<DefaultSampler> BVHNode::sampler;
AxisMethod BVHNode::axisMethod = AxisMethod::LONGESTAXIS;
SplitMethod BVHNode::splitMethod = SplitMethod::MEDIANCUT;
unsigned int BVHNode::maxPrimsPerLeaf = 5;

void BVHNode::setSampler(std::shared_ptr <DefaultSampler> &sampler) { BVHNode::sampler = sampler; }

BVHNode::BVHNode(std::vector<std::shared_ptr<Primitive>> &primitives, size_t begin, size_t end) {
    auto treeSize = end - begin;

    bounds = primitives[begin]->bounds;
    for (auto i = begin; i < end; i++)
        bounds += (primitives[i]->bounds);

    if (treeSize <= maxPrimsPerLeaf) {
        makeLeaf(primitives, begin, end);
        return;
    }
    Aabb centroidBounds = {primitives[begin]->bounds.center(), primitives[begin]->bounds.center()};
    size_t splitAxis;
    Float splitValue;
    size_t secondSetBegin;

    for (auto i = begin; i < end; i++) {
        centroidBounds += primitives[i]->bounds.center();
    }
    if (axisMethod == LONGESTAXIS) {
        splitAxis = centroidBounds.longestAxis();
    } else if (axisMethod == RANDOMAXIS) {
        splitAxis = sampler->uniformInt(0, 2);
    }
    switch (splitMethod) {
        case MIDPOINT: {
            splitValue = centroidBounds.center()[splitAxis];
            auto partitioner = [splitAxis, splitValue](std::shared_ptr<Primitive> &p){ return p->bounds.center()[splitAxis] < splitValue; };
            auto partition = std::partition(primitives.begin() + begin, primitives.begin() + end, partitioner);
            secondSetBegin = partition - primitives.begin();
            // the partition of values with the same value on the split axis can lead to one set being size 0, fall
            // back to median cut then
            if (secondSetBegin != begin && secondSetBegin != end)
                break;
        }
        case MEDIANCUT: {
            auto comparator = [splitAxis](std::shared_ptr<Primitive> &p1,
                                          std::shared_ptr<Primitive> &p2){ return p1->bounds.center()[splitAxis] < p2->bounds.center()[splitAxis]; };
            std::sort(primitives.begin() + begin, primitives.begin() + end, comparator);
            secondSetBegin = begin + treeSize / 2;
        }
    }
    left = std::make_shared<BVHNode>(primitives, begin, secondSetBegin);
    right = std::make_shared<BVHNode>(primitives, secondSetBegin, end);
}

bool BVHNode::hit(Ray &ray, Float tMin, Float tMax, HitRecord *hitRecord) const {
    if (!bounds.hit(ray, tMin, tMax, hitRecord))
        return false;

    if (isLeaf()) {
        bool hit = false;
        for (const Primitive &p: primitives) {
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
}

std::ostream &operator<<(std::ostream &os, const BVHNode &b) {
    if (!b.primitives.empty()) {
        return os << "Leaf (" << b.primitives.size() << ") ";
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

void BVHNode::setConstructionOptions(unsigned int maxPrimsPerLeaf, SplitMethod splitMethod, AxisMethod axisMethod) {
    BVHNode::maxPrimsPerLeaf = maxPrimsPerLeaf;
    BVHNode::splitMethod = splitMethod;
    BVHNode::axisMethod = axisMethod;
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