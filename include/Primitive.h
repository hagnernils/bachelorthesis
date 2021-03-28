//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_PRIMITIVE_H
#define BACHELORTHESIS_PRIMITIVE_H

#include <memory>
#include <utility>
#include <ostream>
#include <random>
#include "Ray.h"
#include "Float.h"
#include "Sampler.h"
#include "MeshObject.h"
#include "HitRecord.h"


class Primitive {
public:
    Primitive();

    Primitive(const Float3& a, const Float3& b, const Float3& c, Float3 n, MeshObject *parent)
            : a(a), b(b), c(c),
              normal(n),
              parent(parent) {
        normal.normalize();
        bounds.min.x = std::min({a.x, b.x, c.x});
        bounds.min.y = std::min({a.y, b.y, c.y});
        bounds.min.z = std::min({a.z, b.z, c.z});
        bounds.max.x = std::max({a.x, b.x, c.x});
        bounds.max.y = std::max({a.y, b.y, c.y});
        bounds.max.z = std::max({a.z, b.z, c.z});
        Float bbPadding = 0.000001;
        bounds.min -= Float3(bbPadding);
        bounds.max += Float3(bbPadding);
    };

    Primitive(const Float3& a, const Float3& b, const Float3& c, MeshObject *parent)
                : Primitive(a, b, c, Float3::cross(b - a, c - a), parent) {};

    bool hit(Ray &ray, Float tMin, Float tMax, HitRecord *hitRecord) const;

    friend std::ostream &operator<<(std::ostream &stream, Primitive &prim);

    bool operator==(const Primitive &rhs) const;

    bool operator!=(const Primitive &rhs) const;

    Float3 sampleArea(const Point2f &p) const;

    inline Float3 atUV(Float u, Float v) const;

    Float Area() const {
        return ((b - a).length() + (c - a).length()) * 0.5;
    }

    Aabb bounds;
    MeshObject *parent = nullptr;
    Float3 a, b, c;
    Float3 normal;
    size_t Id;
};

#endif //BACHELORTHESIS_PRIMITIVE_H