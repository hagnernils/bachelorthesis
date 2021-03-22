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

        #pragma omp parallel for default(none) shared(prims, nodeCount, nodeSurfaceArea, nodeSpectrum, samplesPerPrimitive)
        for (auto &prim : prims) {
            auto primitiveSpectrum = AbsorbedEnergySpectrum(nodeCount + 1);
            for (unsigned int j = 0; j < samplesPerPrimitive; j++) {
                // TODO: get sampleWeight, like this samples are weighted uniformly
                Float pdf = 1. / samplesPerPrimitive ;
                auto estimate = estimateAbsorption(*prim, pdf);
                primitiveSpectrum[estimate.first] += estimate.second * pdf;
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
    float depth = 10;
    return estimateAbsorptionAtRay(hitRecord, ray, depth);
}

std::pair<AbsorptionIndex, Float> Estimator::estimateAbsorptionAtRay(HitRecord &hitRecord, Ray &ray, float &depth) {
    if (--depth >= 0 && scene->closestHit(ray, &hitRecord)) {
        // TODO: check if hit itself
        // TODO: check weighting of these samples! like the *pdf set in http://www.pbr-book.org/3ed-2018/Light_Transport_I_Surface_Reflection/Sampling_Reflection_Functions.html#BSDF::Sample_f
        auto rayInteraction = scene->materials[hitRecord.MaterialIndex].interact(sampler->getCanonical());
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
                return estimateAbsorptionAtRay(hitRecord, ray, depth);
            }
            case SPECULAR_REFLECTION: {
                Float3 reflectedDirection = ray.dir - 2 * (ray.dir.dot(hitRecord.normal) * hitRecord.normal);
                ray = Ray(hitRecord.point, reflectedDirection, ray.intensity);
                return estimateAbsorptionAtRay(hitRecord, ray, depth);
            }
        }
    }

    // if we did not hit anything, add the intensity to the enclosure
    return std::make_pair(scene->emittingNodeCount(), ray.intensity);
}