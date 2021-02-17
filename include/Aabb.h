//
// Created by Nils Hagner on 22.12.20.
//

#ifndef BACHELORTHESIS_AABB_H
#define BACHELORTHESIS_AABB_H
#include "Float.h"
#include "HitRecord.h"
#include "Ray.h"

struct Aabb {
    Float3 min;
    Float3 max;

    Aabb() = default;

    Aabb(const Float3 &min, const Float3 &max) : min(min), max(max) {}
    Aabb(const Float minX, const Float minY, const Float minZ,
         const Float maxX, const Float maxY, const Float maxZ) : min({minX, minY, minZ}), max({maxX, maxY, maxZ}) {}

    bool hit(Ray &ray, Float tMin, Float tMax, HitRecord *hitRecord) const {
        Float inverseDirection[3] = {static_cast<Float>(1. / ray.dir.x), static_cast<Float>(1. / ray.dir.y), static_cast<Float>(1. / ray.dir.z)};

        for (auto i = 0; i < 3; i++) {
            auto t0 = inverseDirection[i] * (min()[i] - ray.base()[i]);
            auto t1 = inverseDirection[i] * (max()[i] - ray.base()[i]);
            if (inverseDirection[i] < 0.0)
                std::swap(t0, t1);
            tMin = t0 > tMin ? t0 : tMin;
            tMax = t1 < tMax ? t1 : tMax;
            if (tMax <= tMin) return false;
        }
        hitRecord->time = tMin;
        return true;
    }

    static Aabb unionAbb(const Aabb &lhs, const Aabb &rhs) {
        return Aabb({std::min(lhs.min.x, rhs.min.x),
                     std::min(lhs.min.y, rhs.min.y),
                     std::min(lhs.min.z, rhs.min.z)},
                    {std::max(lhs.max.x, rhs.max.x),
                     std::max(lhs.max.y, rhs.max.y),
                     std::max(lhs.max.z, rhs.max.z)});
    }

    void operator+=(const Aabb &other) {
        min.x = std::min(min.x, other.min.x);
        min.z = std::min(min.y, other.min.y);
        min.z = std::min(min.z, other.min.z);
        max.x = std::max(max.x, other.max.x);
        max.y = std::max(max.y, other.max.y);
        max.z = std::max(max.z, other.max.z);
    }

    void operator+=(const Float3 &other) {
        min.x = std::min(min.x, other.x);
        min.z = std::min(min.y, other.y);
        min.z = std::min(min.z, other.z);
        max.x = std::max(max.x, other.x);
        max.y = std::max(max.y, other.y);
        max.z = std::max(max.z, other.z);
    }

    // returns 0,1,2 if the x,y,z axis is the longest
    size_t longestAxis() {
        auto distances = {max.x - min.x, max.y - min.y, max.z - min.z};
        return std::distance(distances.begin(), std::max_element(distances.begin(), distances.end()));
    }

    Float3 center() { return min + max * 0.5; }
};

#endif //BACHELORTHESIS_AABB_H
