//
// Created by Nils Hagner on 19.01.21.
//

#include <iostream>
#include "Estimator.h"
//#define VERBOSE
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
                Float pdf = 1. / samplesPerPrimitive;
                auto estimate = estimateAbsorption(*prim, pdf);
                primitiveSpectrum[estimate.first] = primitiveSpectrum[estimate.first] + estimate.second;
            }
            // we assume even emittance over mesh, so scale emitted intensity of primitive with its percentage of the whole
            // node area
            nodeSpectrum += primitiveSpectrum * (prim->Area() / nodeSurfaceArea);
        }

        result[i] = nodeSpectrum.normalize();
    }

    return result;
}

std::pair<AbsorptionIndex, Float> Estimator::estimateAbsorption(Primitive &emittingPrimitive, Float &pdf, bool front) {

    // global direction sampling: one position, one direction
    Float3 primitiveSample = emittingPrimitive.sampleArea(sampler->get2D());

    auto normal = front ? emittingPrimitive.normal : -emittingPrimitive.normal;

    // offset the surface sample into direction of the normal to avoid self intersection
    constexpr Float offsetScale = std::numeric_limits<Float>::epsilon();

    primitiveSample += normal * offsetScale;

    auto directionSample = uniformSampleHemisphereAtNormal(normal, sampler->get2D());

    // the fraction of the emissive power at this point(directional emissive power represented by this ray) is cos theta
    Float directionalEmissivePower = abs(cos(directionSample.dot(normal)));
    pdf *= directionalEmissivePower;

    Ray ray(primitiveSample, directionSample, 1.);
    HitRecord hitRecord{primitiveSample};

    #ifdef VERBOSE
        //#pragma omp critical
        //std::cerr << "emitted from " << ray << " absorbed at " << hitRecord.primitiveIndex << std::endl;
    #pragma omp critical
        std::cout << ray.base.toString() << " " << ray.dir.toString() << std::endl;
    #endif
    auto depth = traceDepth;
    return estimateAbsorptionAtRay(hitRecord, ray, pdf, depth);
}

std::pair<AbsorptionIndex, Float>
Estimator::estimateAbsorptionAtRay(HitRecord &hitRecord, Ray &ray, Float &pdf, float &depth) {
    if (--depth <= 0) {
        pdf = 0;
        return std::make_pair(scene->emittingNodeCount(), 0.);
    }

    // if we did not hit anything, add the intensity to the enclosure
    if (!scene->closestHit(ray, &hitRecord))
        return std::make_pair(scene->emittingNodeCount(), ray.intensity);

    // TODO: check if hit itself
    RaySurfaceInteraction rayInteraction = scene->materials[hitRecord.MaterialIndex].interact(sampler->getCanonical());
    switch (rayInteraction) {
        case ABSORPTION:
#ifdef VERBOSE
            //#pragma omp critical
            //std::cerr << "emitted from " << ray << " absorbed at " << hitRecord.primitiveIndex << std::endl;
#endif
            return std::make_pair(hitRecord.ObjectIndex, ray.intensity);
        case DIFFUSE_REFLECTION: {
            // reflect diffusely with the tangent sphere method
            auto sphereSample = sampler->rejectionSampleUnitSphere();
            Float3 reflectedDirection = hitRecord.point + hitRecord.normal + sphereSample.normalize();
            constexpr Float offsetScale = std::numeric_limits<Float>::epsilon();
            Float3 reflectPosition = hitRecord.point + hitRecord.normal * offsetScale;
            ray = Ray(reflectPosition, reflectedDirection, std::cos(ray.intensity));
            return estimateAbsorptionAtRay(hitRecord, ray, pdf, depth);
        }
        case SPECULAR_REFLECTION: {
            Float3 reflectedDirection = ray.dir - 2 * (ray.dir.dot(hitRecord.normal) * hitRecord.normal);
            ray = Ray(hitRecord.point, reflectedDirection, ray.intensity);
            return estimateAbsorptionAtRay(hitRecord, ray, pdf, depth);
        }
    }
}