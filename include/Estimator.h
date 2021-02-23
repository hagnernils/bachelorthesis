//
// Created by Nils Hagner on 07.01.21.
//

#ifndef BACHELORTHESIS_ESTIMATOR_H
#define BACHELORTHESIS_ESTIMATOR_H

#include "DirectionSampling.h"
#include "Ray.h"
#include "Sampler.h"
#include "Spectrum.h"
#include "Scene.h"
#include "Primitive.h"

typedef size_t AbsorptionIndex;

class Estimator {
public:
    Estimator() = default;

    Estimator(const std::shared_ptr<Scene>& scene) : scene(scene), sampler(scene->sampler) {};

    std::shared_ptr<DefaultSampler> sampler;
    std::shared_ptr<Scene> scene = nullptr;

    const unsigned int samplesPerPrimitive = 10000;

    std::vector<AbsorbedEnergySpectrum> estimateAbsorption();

private:
    // send one global sample and calculate its absorption
    std::pair<size_t, Float> estimateAbsorption(Primitive &emittingPrimitive);

    std::pair<AbsorptionIndex, Float> reflectDiffuse(HitRecord &hitRecord, Ray &ray);

    std::pair<AbsorptionIndex, Float> reflectSpecular(HitRecord &hitRecord, Ray &ray);

    std::pair<AbsorptionIndex, Float> estimateAbsorptionAtRay(HitRecord &hitRecord, Ray &ray);
};

#endif //BACHELORTHESIS_ESTIMATOR_H
