//
// Created by Nils Hagner on 20.12.20.
//

#include "include/Primitive.h"

Primitive::Primitive() {


}

bool Primitive::hit(const Ray &ray, Float tMin, Float tMax, hitRecord &hitRecord) {
    return false;
}

Float3 Primitive::sampleArea() {
    return Float3(0, 0, 0);
}

Float3 Primitive::sampleHemisphere() {
    return Float3(0, 0, 0);
}
