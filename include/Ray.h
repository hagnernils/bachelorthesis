//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_RAY_H
#define BACHELORTHESIS_RAY_H

#include "Float.h"

struct Ray {
    Float3 base;
    Float3 dir;
    Float time = 0;

    Ray() = default;

    Ray(Float3 base, Float3 dir) : base(base), dir(dir) {}

    Float3 at(Float t) const { return base + t * dir; }
};

#endif //BACHELORTHESIS_RAY_H
