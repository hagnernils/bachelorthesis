//
// Created by Nils Hagner on 20.12.20.
//

#include <random>
#include "Sampler.h"
#include "Primitive.h"

Primitive::Primitive() {


}

// Möller, Tomas, and Ben Trumbore. 1997. „Fast, minimum storage ray-triangle intersection“. Journal of Graphics Tools 2 (1): 21–28.
// https://doi.org/10.1080/10867651.1997.10487468.
#define EPS 0.00001
inline bool muellerTrumboreTriangleIntersect(const Float3 rayOrigin, const Float3 rayDir, Float &rayTime,
                                             const Float3 v0, const Float3 v1, const Float3 v2 , Float &u, Float &v) {
    Float3 edge1, edge2, t, p, q;
    Float det, invDet;

    edge1 = v1 - v0;
    edge2 = v2 - v0;

    p = Float3 ::cross(rayDir, edge2);

    det = edge1.dot(p);

    if (det > -EPS && det < EPS)
        return false;
    invDet = 1.0 / det;

    t = rayOrigin - v0;

    u = t.dot(p);
    if (u < 0.0 || u > 1.0)
        return false;

    q = Float3::cross(t, edge1);

    v = rayDir.dot(q) * invDet;
    if (v < 0.0 || u + v > 1.0)
        return false;

    rayTime = edge2.dot(q) * invDet;
    return true;
}

// see Turk, Greg. 1990. „Generating random points in triangles“. In Graphics gems, 24–28. USA: Academic Press Professional, Inc.
// 1st proposed version
inline Float3 sampleTriangle1(const Float3 &a, const Float3 &b, const Float3 &c, Float s, Float t) {
    Float squareT = std::sqrt(t);
    return (1 - squareT) * a + (1 - s) * b + (s * squareT) * c;
}

// see Turk, Greg. 1990. „Generating random points in triangles“. In Graphics gems, 24–28. USA: Academic Press Professional, Inc.
// 2nd proposed version
inline Float3 sampleTriangle2(const Float3 &a, const Float3 &b, const Float3 &c, Float s, Float t) {
    if (s + t > 1) {
        s = 1 - s;
        t = 1 - t;
    }
    Float A = 1 - s - t;
    return A * a + s * b + t * c;
}

bool Primitive::hit(Ray &ray, Float tMin, Float tMax, HitRecord &hitRecord) const {
    Float u, v;

    bool result = muellerTrumboreTriangleIntersect(ray.base, ray.dir, ray.time, a, b, c, u, v);

    return result && tMin < ray.time && ray.time < tMax;
}

inline Float3 Primitive::sampleArea(const Point2f &p) const {
    return sampleTriangle2(a, b, c, p.first, p.second);
}

Float3 Primitive::sampleHemisphere() {
    return Float3(0, 0, 0);
}

std::ostream &operator<<(std::ostream &stream, Primitive &prim) {
    return stream << "Primitive {" << std::endl
                  << "A: " << prim.a << std::endl
                  << "B: " << prim.b << std::endl
                  << "C: " << prim.c << std::endl
                  << "Normal: " << prim.normal << std::endl
                  << "Parent: " << prim.parent << std::endl
                  << "}" << std::endl;
}
