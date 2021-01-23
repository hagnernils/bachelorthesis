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


class Estimator {
public:
    Estimator() = default;

    DefaultSampler sampler;
    Scene *scene = nullptr;

    const unsigned int samplesPerPrimitive = 10000;

    std::vector<AbsorbedEnergySpectrum> estimateAbsorption();

private:
    // send one global sample and calculate its absorption
    std::pair<size_t, Float> estimateAbsorption(Primitive &emittingPrimitive);
};

#endif //BACHELORTHESIS_ESTIMATOR_H
