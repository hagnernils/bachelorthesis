//
// Created by Nils Hagner on 25.12.20.
//

#include <fstream>
#include "gtest/gtest.h"
#include "Sampler.h"
#include "DirectionSampling.h"

TEST(DirectionSampling, uniformSampleHemisphere) {
    DefaultSampler sampler(1234);
    auto res1 = uniformSampleHemisphere(sampler.get2D());
    EXPECT_FLOAT_EQ(res1.x, 0.21607839);
    EXPECT_FLOAT_EQ(res1.y, -0.074387543);
    EXPECT_FLOAT_EQ(res1.z, 0.97353822);
}