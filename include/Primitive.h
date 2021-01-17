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
#include "HitRecord.h"

// forward declaration of Object to be used as parent of a primitive
class MeshObject;

class Primitive {
public:
    Primitive();

    Primitive(Float3 a, Float3 b, Float3 c, Float3 n, std::shared_ptr<MeshObject> parent)
            : a(a), b(b), c(c),
              normal(n),
              parent(std::move(parent)) { normal.normalize(); };

    Primitive(Float3 a, Float3 b, Float3 c, std::shared_ptr<MeshObject> parent)
            : Primitive(a, b, c,
                        Float3::cross(b - a, c - a),
                        std::move(parent)) {};

    bool hit(Ray &ray, Float tMin, Float tMax, HitRecord &hitRecord) const;

    friend std::ostream &operator<<(std::ostream &stream, Primitive &prim);

    Float3 sampleArea(const Point2f &p) const;

    static Float3 sampleHemisphere(const Point2f &diskSample);

    std::shared_ptr<MeshObject> parent;
    Float3 a, b, c;
    Float3 normal;
};

#endif //BACHELORTHESIS_PRIMITIVE_H