//
// Created by Nils Hagner on 13.01.21.
//

#ifndef BACHELORTHESIS_HITRECORD_H
#define BACHELORTHESIS_HITRECORD_H
#include "Float.h"

// TODO: abstract like in http://www.pbr-book.org/3ed-2018/Introduction/pbrt_System_Overview.html
struct HitRecord {
    Float3 point;
    Float3 normal;
    Float time;
    size_t ObjectIndex;  // the object that was hit
    size_t primitiveIndex;  // the prim that was hit
    size_t MaterialIndex;
    Float u, v;
};

#endif //BACHELORTHESIS_HITRECORD_H
