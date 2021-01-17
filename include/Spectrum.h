//
// Created by Nils Hagner on 13.01.21.
//

#ifndef BACHELORTHESIS_SPECTRUM_H
#define BACHELORTHESIS_SPECTRUM_H
#include <valarray>
#include "Float.h"

struct AbsorbedEnergySpectrum {
    size_t count;
    std::valarray<Float> bins;

    explicit AbsorbedEnergySpectrum(size_t count) : count(count) {
        bins = std::valarray(0.f, count);
    }

    Float operator[](size_t index) const {
        return bins[index];
    }

    Float &operator[](size_t index) {
        return bins[index];
    }

    void operator+=(AbsorbedEnergySpectrum &other) {
        bins += other.bins;
    }

    void operator+=(AbsorbedEnergySpectrum &&other) {
        bins += other.bins;
    }
};

#endif //BACHELORTHESIS_SPECTRUM_H
