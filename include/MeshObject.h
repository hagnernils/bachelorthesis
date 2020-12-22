//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_MESHOBJECT_H
#define BACHELORTHESIS_MESHOBJECT_H
#include <string>
#include <vector>
#include "Primitive.h"

class MeshObject {
public:
    std::string name;
    // the material in the scene that object is assigned
    size_t materialIndex;

    std::vector<Primitive>& primitives;
    std::vector<Primitive> getPrimitives();

    MeshObject();
};


#endif //BACHELORTHESIS_MESHOBJECT_H
