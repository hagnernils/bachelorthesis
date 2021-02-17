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

inline Float3 uniformSampleHemisphereAtNormal(const Float3 &surfaceNormal, const Point2f &p) {
    Float3 sample = uniformSampleHemisphere(p);
    // create an orthonormal basis (s|t|n) of current surface point
    // see https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html#orthonormalbases/generatinganorthonormalbasis
    Float3 s, t;
    Float3 a = fabs(surfaceNormal.x) > 0.9 ? Float3 (0,1,0) : Float3 (1,0,0);
    t = Float3::cross(surfaceNormal, a);
    t.normalize();
    s = Float3::cross(surfaceNormal, t);

    return sample.x * s + sample.y * t + sample.z * surfaceNormal;
}

constexpr Float uniformHemispherePdf() { return 2 * M_1_PI; }

inline Float cosineHemispherePdf(Float angleToNormal) { return angleToNormal * M_1_PI; }

#endif //BACHELORTHESIS_DIRECTIONSAMPLING_H
