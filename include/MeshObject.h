//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_MESHOBJECT_H
#define BACHELORTHESIS_MESHOBJECT_H

#include <string>
#include <vector>
#include "Aabb.h"
#include "Buffer.h"
#include "Matrix.h"
#include "Primitive.h"

class MeshObject {
public:
    std::string name;
    // the material in the scene that object is assigned
    size_t materialIndex = 0;
    Matrix4x4 transform;
    Aabb aabb;
    std::vector<BufferView<u_char>> indices;
    std::vector<BufferView<Float3>> positions;
    std::vector<BufferView<Float3>> normals;

    MeshObject();

    // linearize this mesh into primitives we can sample
    std::vector<Primitive> toPrimitives();
};


#endif //BACHELORTHESIS_MESHOBJECT_H
