//
// Created by Nils Hagner on 24.12.20.
//

#include "gtest/gtest.h"
#include "Float.h"

TEST(Float, FloatExtractionOperator) {
    Float3 f = {0, 1, 1 / 3.f};
    std::stringstream ss;
    ss << f;
    EXPECT_EQ(ss.str(), "Float3 {0.00000000, 1.00000000, 0.33333334}");
}