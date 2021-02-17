//
// Created by Nils Hagner on 13.01.21.
//

#ifndef BACHELORTHESIS_HITRECORD_H
#define BACHELORTHESIS_HITRECORD_H
#include "Float.h"

enum RaySurfaceInteraction {
    ABSORPTION = 0,
    DIFFUSE_REFLECTION = 1,
    SPECULAR_REFLECTION = 2
};

// TODO: abstract like in http://www.pbr-book.org/3ed-2018/Introduction/pbrt_System_Overview.html
struct HitRecord {
    Float3 point;
    Float3 normal;
    Float time;
    size_t ObjectIndex;  // the object that was hit
    size_t MaterialIndex;
    Float u, v;
};

#endif //BACHELORTHESIS_HITRECORD_H
