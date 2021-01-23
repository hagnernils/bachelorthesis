//
// Created by Nils Hagner on 19.01.21.
//

#include "Estimator.h"

std::vector<AbsorbedEnergySpectrum> Estimator::estimateAbsorption() {
    const auto nodeCount = scene->nodeCount();
    std::vector<AbsorbedEnergySpectrum> result(nodeCount, AbsorbedEnergySpectrum(nodeCount));

    #pragma omp parallel for default(none) shared(result, nodeCount)
    for (unsigned int i = 0; i < nodeCount; i++) {
        auto prims = scene->objects[i].toPrimitives();
        auto nodeSpectrum = AbsorbedEnergySpectrum(nodeCount);

        Float nodeSurfaceArea = 0;
        for (Primitive &p : prims)
            nodeSurfaceArea += p.Area();

        for (auto &prim : prims) {
            auto primitiveSpectrum = AbsorbedEnergySpectrum(nodeCount);

            for (unsigned int j = 0; j < samplesPerPrimitive; j++) {
                auto estimate = estimateAbsorption(prim);
                #pragma omp atomic
                primitiveSpectrum[estimate.first] += estimate.second;
            }
            // we assume even emittance over mesh, so scale emitted energy of primitive with its percentage of the whole
            // node area
            nodeSpectrum += primitiveSpectrum * (prim.Area() / nodeSurfaceArea);
        }

        result[i] = nodeSpectrum.normalize();
    }

    return result;
}

std::pair<size_t, Float> Estimator::estimateAbsorption(Primitive &emittingPrimitive) {

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
        return std::make_pair(hitRecord.ObjectIndex, rayEnergy);
    }
    return std::make_pair(0, 0);
}
