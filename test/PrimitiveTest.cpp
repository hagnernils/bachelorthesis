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
                               parent);

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

    bool result = prim.hit(ray, -1, 2, hr);
    EXPECT_EQ(result, true);
    EXPECT_FLOAT_EQ(ray.time, 1);
}