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

        //std::cerr << "primitive " << scene->objects[i].name << " with Area " << nodeSurfaceArea << std::endl;

        #pragma omp parallel for default(none) shared(prims, nodeCount, nodeSurfaceArea, nodeSpectrum, std::cout)
        for (auto &prim : prims) {
            auto primitiveSpectrum = AbsorbedEnergySpectrum(nodeCount + 1);
            int N = samplesPerPrimitive, M = 1;
            auto normal = prim->normal;
            constexpr Float offsetScale = 10 * std::numeric_limits<Float>::epsilon();
            for (int areaSamples = 0; areaSamples < N; areaSamples++) {

                Float3 primitiveSample = prim->sampleArea(sampler->get2D());
                // offset the surface sample into direction of the normal to avoid self intersection
                primitiveSample += normal * offsetScale;

                for (int directionSamples = 0; directionSamples < M; directionSamples++) {
                    Float pdf;
                    Float3 directionSample = sampleHemisphereAtNormal(normal, sampler, pdf, useUniformSampling);
                    // the fraction of the emissive power at this point(directional emissive power represented by this ray) is cos theta
                    Float cosTheta = std::abs(directionSample.dot(normal));
                    Float intensity = useUniformSampling ? 1. : (cosTheta / M_PI) / pdf;
                    Ray ray(primitiveSample, directionSample, 1.);
                    HitRecord hitRecord{};

                    auto depth = traceDepth;
                    auto estimate = estimateAbsorptionAtRay(hitRecord, ray, depth);
                    primitiveSpectrum[estimate.first] += estimate.second;
                }
            }
            // we assume even emittance over mesh, so scale emitted intensity of primitive with its percentage of the whole
            // node area
            primitiveSpectrum /= (N * M);
            nodeSpectrum += primitiveSpectrum * (prim->Area() / nodeSurfaceArea);
        }

        result[i] = nodeSpectrum;
    }

    return result;
}

std::pair<AbsorptionIndex, Float>
Estimator::estimateAbsorptionAtRay(HitRecord &hitRecord, Ray &ray, float &depth) {
    if (--depth <= 0) {
        return std::make_pair(scene->emittingNodeCount(), 0.);
    }

    // if we did not hit anything, add the intensity to the enclosure
    if (!scene->closestHit(ray, &hitRecord))
        return std::make_pair(scene->emittingNodeCount(), ray.intensity);

    auto material = scene->materials[hitRecord.MaterialIndex];
    constexpr Float offsetScale = std::numeric_limits<Float>::epsilon();
    Float3 reflectPosition = hitRecord.point + hitRecord.normal * offsetScale;
    auto normal = hitRecord.normal.dot(ray.dir) > 0 ? -hitRecord.normal : hitRecord.normal;
    switch (material.interact(sampler->getCanonical())) {
        case ABSORPTION:
//#define VERBOSE
#ifdef VERBOSE
            #pragma omp critical
            //std::cerr << "emitted from " << ray << " absorbed at " << hitRecord.primitiveIndex << std::endl;
            std::cout << ray.base.toString() << " " << (hitRecord.point - ray.base).toString() << std::endl;
#endif
            return std::make_pair(hitRecord.ObjectIndex, ray.intensity);
        case DIFFUSE_REFLECTION: {
            Float pdf = 1.;
            Float3 reflectedDirection = sampleHemisphereAtNormal(normal, sampler, pdf, false);
            const Float diffuseScatterBRDF = M_1_PI;
            const Float cosTheta = std::abs(reflectedDirection.dot(normal));
            ray = Ray(reflectPosition, reflectedDirection, ray.intensity);
            auto result = estimateAbsorptionAtRay(hitRecord, ray, depth);
            return  std::make_pair(result.first, result.second);
        }
        case SPECULAR_REFLECTION: {
            // the pdf of specular reflection cancels out with the cosine term in the reflection equation,
            // for reflectedDirection the intensity does not change
            Float3 reflectedDirection = ray.dir - 2 * (ray.dir.dot(normal) * normal);
            ray = Ray(reflectPosition, reflectedDirection, ray.intensity);
            return estimateAbsorptionAtRay(hitRecord, ray, depth);
        }
    }
}