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
    // TODO: maybe call this intensity bc it is along the differential area normal to the ray?
    Float intensity = 0;

    Ray() = default;

    Ray(Float3 base, Float3 dir) : base(base), dir(dir) {}
    Ray(Float3 base, Float3 dir, Float energy) : base(base), dir(dir), intensity(energy) {}
};

#endif //BACHELORTHESIS_RAY_H
