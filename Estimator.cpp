//
// Created by Nils Hagner on 19.01.21.
//

#include <iostream>
#include "Estimator.h"

std::vector<AbsorbedEnergySpectrum> Estimator::estimateAbsorption() {
    const auto nodeCount = scene->emittingNodeCount();
    std::vector<AbsorbedEnergySpectrum> result(nodeCount, AbsorbedEnergySpectrum(nodeCount + 1));

    for (unsigned int i = 0; i < scene->objects.size(); i++) {
        auto prims = scene->primsOfObject[i];
        auto nodeSpectrum = AbsorbedEnergySpectrum(nodeCount + 1);

        Float nodeSurfaceArea = 0;
        for (auto &p : prims)
            nodeSurfaceArea += p->Area();

        std::cerr << "primitive " << scene->objects[i].name << " with Area " << nodeSurfaceArea << std::endl;

        #pragma omp parallel for default(none) shared(prims, nodeCount, nodeSurfaceArea, nodeSpectrum)
        for (auto &prim : prims) {
            auto primitiveSpectrum = AbsorbedEnergySpectrum(nodeCount + 1);
            for (unsigned int j = 0; j < samplesPerPrimitive; j++) {
                // TODO: get sampleWeight, like this samples are weighted uniformly
                auto estimate = estimateAbsorption(*prim);
                primitiveSpectrum[estimate.first] += estimate.second;
            }
            // we assume even emittance over mesh, so scale emitted energy of primitive with its percentage of the whole
            // node area
            nodeSpectrum += primitiveSpectrum * (prim->Area() / nodeSurfaceArea);
        }

        result[i] = nodeSpectrum.normalize();
    }

    return result;
}

std::pair<AbsorptionIndex, Float> Estimator::estimateAbsorption(Primitive &emittingPrimitive) {

    // global direction sampling: one position, one direction
    emittingPrimitive.sampleArea(sampler->get2D());
    Float3 primitiveSample = emittingPrimitive.sampleArea(sampler->get2D());

    // offset the surface sample into direction of the normal to avoid self intersection
    constexpr Float offsetScale = std::numeric_limits<Float>::epsilon();
    primitiveSample += emittingPrimitive.normal * offsetScale;

    auto directionSample = uniformSampleHemisphere(sampler->sampleDisk());
    // flip the direction to be in line with the normal
    if (directionSample.dot(emittingPrimitive.normal) < 0.f)
        directionSample = -directionSample;

    // TODO: determine wether this is intensity or not, bc this is direction emissive power
    Float rayEnergy = cos(directionSample.dot(emittingPrimitive.normal));

    Ray ray(primitiveSample, directionSample, rayEnergy);
    HitRecord hitRecord{primitiveSample};

    return estimateAbsorptionAtRay(hitRecord, ray);
}

std::pair<AbsorptionIndex, Float> Estimator::estimateAbsorptionAtRay(HitRecord &hitRecord, Ray &ray) {
    if (scene->closestHit(ray, &hitRecord)) {
        // TODO: check if hit itself
        // TODO: check weighting of these samples! like the *pdf set in http://www.pbr-book.org/3ed-2018/Light_Transport_I_Surface_Reflection/Sampling_Reflection_Functions.html#BSDF::Sample_f
        auto rayInteraction = scene->materials[hitRecord.MaterialIndex].interact(sampler->getCanonical());
        switch (rayInteraction) {
            case ABSORPTION:
#ifdef VERBOSE
                #pragma omp critical
                std::cerr << "emitted from " << ray.emitPrimId << " absorbed at " << hitRecord.primitiveIndex << std::endl;
                #pragma omp critical
                std::cout << ray.base.toString() << " " << ray.dir.toString() << std::endl;
#endif
                return std::make_pair(hitRecord.ObjectIndex, ray.energy);
            case DIFFUSE_REFLECTION:
                return reflectDiffuse(hitRecord, ray);
            case SPECULAR_REFLECTION:
                return reflectSpecular(hitRecord, ray);
        }
    }

    // if we did not hit anything, add the energy to the enclosure
    return std::make_pair(scene->emittingNodeCount(), ray.energy);
}

// reflect diffusely with the tangent sphere method
std::pair<AbsorptionIndex, Float> Estimator::reflectDiffuse(HitRecord &hitRecord, Ray &ray) {
    auto cubeSample = sampler->rejectionSampleUnitSphere();
    Float3 reflectedDirection = hitRecord.point + hitRecord.normal + cubeSample.normalize();
    ray = Ray(hitRecord.point, reflectedDirection, std::cos(ray.energy));
    return estimateAbsorptionAtRay(hitRecord, ray);
}

std::pair<AbsorptionIndex, Float> Estimator::reflectSpecular(HitRecord &hitRecord, Ray &ray) {
    Float3 reflectedDirection = ray.dir - 2 * (ray.dir.dot(hitRecord.normal) * hitRecord.normal);
    ray = Ray(hitRecord.point, reflectedDirection);
    return estimateAbsorptionAtRay(hitRecord, ray);
}
