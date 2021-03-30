//
// Created by Nils Hagner on 25.12.20.
//

#ifndef BACHELORTHESIS_DIRECTIONSAMPLING_H
#define BACHELORTHESIS_DIRECTIONSAMPLING_H

#include <cmath>
#include <random>
#include <utility>
#include <memory>
#include "Float.h"
#include "Sampler.h"


constexpr Float uniformHemispherePdf() { return 1. / 2 * M_PI; }
constexpr Float uniformHemisphereInvPdf() { return 2. * M_PI; }

constexpr Float uniformSpherePdf() { return 1. / 4. * M_PI; }
constexpr Float uniformSphereInvPdf() { return 4 * M_PI; }

inline Float cosineHemispherePdf(Float angleToNormal) { return angleToNormal * M_1_PI; }
inline Float cosineHemisphereInvPdf(Float angleToNormal) { return angleToNormal < 0 ? 0 : angleToNormal * M_PI; }

// Malleys Method to project a disk ssample up into the hemisphere
inline Float3 cosineSampleHemisphereMalley(const Point2f diskSample) {
    // project a sample on a circle up to the hemisphere
    Float height = std::sqrt(std::max((Float)0., (Float)1. - diskSample.first * diskSample.first - diskSample.second * diskSample.second));
    return Float3(diskSample.first, diskSample.second, height);
}

// short version of Malleys method
inline Float3 cosineSampleHemisphere(const Point2f p) {
    Float r = std::sqrt(p.first);
    Float theta = 2 * M_PI * p.second;

    return Float3(r * std::cos(theta), r * std::sin(theta), std::sqrt(std::max((Float)0., 1. - p.first)));
}

inline Float3 uniformSampleHemisphere(const Point2f &p) {
    Float p2 = p.second;
    Float phi = 2 * M_PI * p.first;
    Float scale = std::sqrt(p2);
    Float z = std::sqrt(1 - p2);
    return Float3(scale * std::cos(phi), scale * std::sin(phi), z);
}

// creating and rotating a hemisphere sample, returning the associated PDF. Technique for rotation by Peter Shirley
inline Float3 sampleHemisphereAtNormal(const Float3 &surfaceNormal, std::shared_ptr<DefaultSampler> sampler, Float &pdf, bool uniform) {
    auto sample = uniform ? uniformSampleHemisphere(sampler->get2D()) : cosineSampleHemisphereMalley(sampler->sampleDisk());

    // create an orthonormal basis (s|t|n) of current surface point
    // see https://raytracing.github.io/books/RayTracingTheRestOfYourLife.html#orthonormalbases/generatinganorthonormalbasis
    Float3 s, t;
    Float3 a = fabs(surfaceNormal.x) > 0.9 ? Float3 (0,1,0) : Float3 (1,0,0);
    t = Float3::cross(surfaceNormal, a);
    t.normalize();
    s = Float3::cross(surfaceNormal, t);
    s.normalize();

    sample = sample.x * s + sample.y * t + sample.z * surfaceNormal;

    pdf = uniform ? uniformHemispherePdf() : cosineHemispherePdf(std::abs(sample.dot(surfaceNormal)));

    return sample;
}


#endif //BACHELORTHESIS_DIRECTIONSAMPLING_H
