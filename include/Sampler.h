//
// Created by Nils Hagner on 25.12.20.
//

#ifndef BACHELORTHESIS_SAMPLER_H
#define BACHELORTHESIS_SAMPLER_H

#include <random>
#include <utility>
#include "Float.h"

typedef std::pair<float, float> Point2f;

template<class Generator, size_t precision>
class Sampler {
public:
    Sampler() {
        Generator gen(randomDevice());
        generator = gen;
    }

    explicit Sampler(size_t seed) {
        Sampler<Generator, precision>();
        this->seed(seed);
    }

    inline Float getCanonical() {  return std::generate_canonical<Float, precision>(generator); }

    inline Point2f get2D() {
        auto u = getCanonical();
        auto v = getCanonical();
        return std::make_pair(u, v);
    }

    inline Float3 rejectionSampleUnitSphere() {
        while (true) {
            auto dir = randomVector3();
            if (dir.length() < 1)
                return dir;
        }
    }

    inline Point2f sampleDisk() {
        return rejectionSampleDisk();
    }

    inline int uniformInt(int lower, int upper) {
        std::uniform_int_distribution<> distribution(lower, upper);
        return distribution(generator);
    }

    void seed(size_t seed) {
        generator.seed(seed);  // NOLINT (cert-msc51-cpp)
    }

private:
    std::random_device randomDevice;
    Generator generator;

    inline Point2f rejectionSampleDisk() {
        auto u = getCanonical();
        auto v = getCanonical();
        if (u * u + v * v < 1)
            return std::make_pair(u, v);
        return rejectionSampleDisk();
    }

    inline Float3 randomVector3() {
        return Float3(2 * getCanonical() - 1, 2 * getCanonical() - 1, 2 * getCanonical() - 1);
    }
};

typedef Sampler<std::mt19937_64, 10> DefaultSampler;

#endif //BACHELORTHESIS_SAMPLER_H
