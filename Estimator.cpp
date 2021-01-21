//
// Created by Nils Hagner on 19.01.21.
//

#include "Estimator.h"

std::vector<AbsorbedEnergySpectrum> Estimator::estimateAbsorption() {
    auto nodeCount = scene->nodeCount();
    std::vector<AbsorbedEnergySpectrum> result(nodeCount, AbsorbedEnergySpectrum(nodeCount));

    for (unsigned int i = 0; i < nodeCount; i++) {
        auto prims = scene->objects[i].toPrimitives();
        auto nodeSpectrum = AbsorbedEnergySpectrum(nodeCount);

        Float nodeSurfaceArea = 0;
        std::for_each(prims.begin(), prims.end(),
                      [&nodeSurfaceArea](Primitive &p) { nodeSurfaceArea += p.Area(); });

        for (auto &prim : prims) {
            auto primitiveSpectrum = AbsorbedEnergySpectrum(nodeCount);
            for (unsigned int j = 0; j < samplesPerPrimitive; j++)
                primitiveSpectrum += estimateAbsorption(prim);

            // we assume even emittance over mesh, so scale emitted energy of primitive with its percentage of the whole
            // node area
            nodeSpectrum += primitiveSpectrum * (prim.Area() / nodeSurfaceArea);
        }

        result[i] = nodeSpectrum.normalize();
    }

    return result;
}

AbsorbedEnergySpectrum Estimator::estimateAbsorption(Primitive &emittingPrimitive) {
    // TODO: to insert energy into matching bin, get object from primtiive id?
    auto spectrum = AbsorbedEnergySpectrum(scene->nodeCount());

    // global direction sampling: one position, one direction
    emittingPrimitive.sampleArea(sampler.get2D());
    Float3 primitiveSample = emittingPrimitive.sampleArea(sampler.get2D());

    // offset the surface sample into direction of the normal to avoid self intersection
    constexpr Float offsetScale = std::numeric_limits<Float>::epsilon();
    primitiveSample += emittingPrimitive.normal * offsetScale;
    auto directionSample = uniformSampleHemisphere(sampler.sampleDisk());
    // flip the direction to be in line with the normal
    if (directionSample.dot(emittingPrimitive.normal) < 0.f)
        directionSample = -directionSample;

    Float rayEnergy = std::abs(cos(directionSample.dot(emittingPrimitive.normal)));

    Ray ray(primitiveSample, directionSample);
    HitRecord hitRecord{};

    if (scene->closestHit(ray, &hitRecord)) {
        // TODO: check if hit itself
        hitRecord.MaterialIndex = emittingPrimitive.parent->materialIndex;
        spectrum[hitRecord.ObjectIndex] += rayEnergy;
    }

    return spectrum;
}
