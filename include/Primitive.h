//
// Created by Nils Hagner on 20.12.20.
//

#pragma once
#include <memory>
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
    bool hit(const Ray& ray, Float tMin, Float tMax, hitRecord& hitRecord);
    Float3 sampleArea();
    static Float3 sampleHemisphere();
private:
    std::shared_ptr<MeshObject> parent;
    Float3 a, b, c;
    Float3 normal;
};