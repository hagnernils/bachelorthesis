//
// Created by Nils Hagner on 13.01.21.
//

#ifndef BACHELORTHESIS_HITRECORD_H
#define BACHELORTHESIS_HITRECORD_H



struct HitRecord {
    Float3 point;
    Float3 normal;
    Float time;
    size_t PrimitiveIndex;
    size_t MaterialIndex;
    Float u, v;
};

#endif //BACHELORTHESIS_HITRECORD_H
