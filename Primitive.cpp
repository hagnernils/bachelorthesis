//
// Created by Nils Hagner on 20.12.20.
//

#include "include/Primitive.h"

Primitive::Primitive() {


}

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
inline Float3 sampleTriangle(const Primitive *prim, Float s, Float t) {
    if (s + t > 1) {
        s = 1 - s;
        t = 1 - t;
    }

    Float A = 1 - s - t;
    return A * prim->a + s * prim->b + t * prim->c;
}

bool Primitive::hit(Ray &ray, Float tMin, Float tMax, HitRecord &hitRecord) const {
    Float u, v;

    bool result = muellerTrumboreTriangleIntersect(ray.base, ray.dir, ray.time, a, b, c, u, v);

    return result && tMin < ray.time && ray.time < tMax;
}

Float3 Primitive::sampleArea() {
    return Float3(0, 0, 0);
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
