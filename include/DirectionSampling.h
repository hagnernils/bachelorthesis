//
// Created by Nils Hagner on 25.12.20.
//

#ifndef BACHELORTHESIS_DIRECTIONSAMPLING_H
#define BACHELORTHESIS_DIRECTIONSAMPLING_H

#include <cmath>
#include <random>
#include <utility>
#include "Float.h"
#include "Sampler.h"


inline Float3 cosineSampleHemisphere(const Point2f diskSample) {
    // project a sample on a circle up to the hemisphere
    Float height = std::sqrt(1 - diskSample.first * diskSample.first - diskSample.second * diskSample.second);
    return Float3(diskSample.first, diskSample.second, height);
}

inline Float3 uniformSampleHemisphere(const Point2f &p) {
    Float p2 = p.second;
    Float phi = 2 * M_PI * p.first;
    Float scale = std::sqrt(p2);
    Float z = std::sqrt(1 - p2);
    return Float3(scale * std::cos(phi), scale * std::sin(phi), z);
}

constexpr Float uniformHemispherePdf() { return 2 * M_1_PI; }

inline Float cosineHemispherePdf(Float angleToNormal) { return angleToNormal * M_1_PI; }

#endif //BACHELORTHESIS_DIRECTIONSAMPLING_H
