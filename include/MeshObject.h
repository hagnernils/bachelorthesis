//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_MESHOBJECT_H
#define BACHELORTHESIS_MESHOBJECT_H
#include <string>
#include <vector>
#include "Buffer.h"
#include "Matrix.h"
#include "Primitive.h"

class MeshObject {
public:
    std::string name;
    // the material in the scene that object is assigned
    size_t materialIndex = 0;
    Matrix4x4 transform;

    std::vector<BufferView<u_char>> indices;
    std::vector<BufferView<u_char>> positions;
    std::vector<BufferView<u_char>> normals;

    MeshObject();
    MeshObject(std::string name, size_t materialIndex);
};


#endif //BACHELORTHESIS_MESHOBJECT_H
