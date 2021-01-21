//
// Created by Nils Hagner on 13.01.21.
//

#ifndef BACHELORTHESIS_SPECTRUM_H
#define BACHELORTHESIS_SPECTRUM_H
#include <ostream>
#include <valarray>
#include "Float.h"

struct AbsorbedEnergySpectrum {
    size_t count;

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

    void operator/=(Float val) {
        bins /= val;
    }

    AbsorbedEnergySpectrum &operator*(Float val) {
        bins *= val;
        return *this;
    }

    Float sum() {
        return bins.sum();
    }

    AbsorbedEnergySpectrum &normalize() {
        bins /= bins.sum();
        return *this;
    }

    friend std::ostream &operator<<(std::ostream &os, const AbsorbedEnergySpectrum &spectrum) {
        os << "Spectrum { ";
        os << std::setprecision(8) << std::fixed << std::showpoint;
        for (auto &v : spectrum.bins) {
            os << v << " ";
        }
        return os << "}" << std::endl;
    }

private:
    std::valarray<Float> bins;
};
#endif //BACHELORTHESIS_SPECTRUM_H
