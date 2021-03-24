//
// Created by Nils Hagner on 24.12.20.
//

#include <random>
#include "gtest/gtest.h"
#include "../Primitive.cpp"
#include "../MeshObject.cpp"

// The normal calculation should follow the right-hand-rule
// that means in a z-up coordinate system z is the normal of the x and y axes
TEST (Primtive, PrimitiveConstructorNormalCalculation) {
    auto parent = std::make_shared<MeshObject>();
    Primitive prim = Primitive(Float3(0, 0, 0),
                               Float3(1, 0, 0),
                               Float3(0, 1, 0),
                               parent.get());

    EXPECT_EQ(prim.normal, Float3(0, 0, 1));

}

// make sure a ray traced down the z axis intersects an half-unit-square triangle
TEST (Primtive, MuellerTrumboreTest) {
    Primitive prim;
    prim.a = Float3(0, 0, 0);
    prim.b = Float3(1, 0, 0);
    prim.c = Float3(0, 1, 0);

    Ray ray;
    ray.base = Float3(0.2, 0.2, 1);
    ray.dir = Float3(0, 0, -1);

    Float u, v;

    bool result = muellerTrumboreTriangleIntersect(ray.base, ray.dir, ray.time,
                                                   prim.a, prim.b, prim.c, u, v);
    EXPECT_EQ(result, true);
    EXPECT_FLOAT_EQ(ray.time, 1);
    EXPECT_FLOAT_EQ(u, 0.2);
    EXPECT_FLOAT_EQ(v, 0.2);
}

TEST (Primtive, TriangleHitTMinTMax) {
    // test the t min and t max functionality
    Primitive prim;
    prim.a = Float3(0, 0, 0);
    prim.b = Float3(1, 0, 0);
    prim.c = Float3(0, 1, 0);

    Ray ray = Ray(Float3(0.2, 0.2, 1), Float3(0, 0, -1));

    HitRecord hr = HitRecord();

    bool result = prim.hit(ray, -1, 2, &hr);
    EXPECT_EQ(result, true);
    EXPECT_FLOAT_EQ(hr.time, 1);
}

TEST (Primtive, TriangleSampleArea) {
    // test the sampling
    Primitive prim;
    prim.a = Float3(0, 0, 0);
    prim.b = Float3(1, 0, 0);
    prim.c = Float3(0, 1, 0);

    DefaultSampler sampler(1234);
    auto result = prim.sampleArea(sampler.get2D());

    EXPECT_FLOAT_EQ(result.x, 0.94723159);
    EXPECT_FLOAT_EQ(result.y, 0.052223373);
    EXPECT_FLOAT_EQ(result.z, 0);

    auto result2 = prim.sampleArea(sampler.get2D());

    EXPECT_FLOAT_EQ(result2.x, 0.025681734);
    EXPECT_FLOAT_EQ(result2.y, 0.054251611);
    EXPECT_FLOAT_EQ(result2.z, 0);
}