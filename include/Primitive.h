//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_PRIMITIVE_H
#define BACHELORTHESIS_PRIMITIVE_H

#include <memory>
#include <utility>
#include <ostream>
#include "Ray.h"
#include "Float.h"

// forward declaration of Object to be used as parent of a primitive
class MeshObject;

struct hitRecord {
    Float3 point;
    Float3 normal;
    Float rayTime;
};


class Primitive {
public:
    Primitive();

    Primitive(Float3 a, Float3 b, Float3 c, Float3 normal,
              std::shared_ptr<MeshObject> parent)
            : a(a), b(b), c(c),
              normal(normal),
              parent(std::move(
                      parent)) {};

    bool hit(const Ray &ray, Float tMin, Float tMax, hitRecord &hitRecord);

    friend std::ostream &operator<<(std::ostream &stream, Primitive &prim);

    Float3 sampleArea();

    static Float3 sampleHemisphere();

    std::shared_ptr<MeshObject> parent;
    Float3 a, b, c;
    Float3 normal;
};

#endif //BACHELORTHESIS_PRIMITIVE_H