//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_MESHOBJECT_H
#define BACHELORTHESIS_MESHOBJECT_H

#include <string>
#include <vector>
#include <memory>
#include "Aabb.h"
#include "Buffer.h"
#include "Matrix.h"

// forward declaration of Primitive class
class Primitive;

class MeshObject {
public:
    std::string name;
    // the material in the scene that object is assigned
    size_t materialIndex = 0;
    size_t objectID = 0; // TODO: use this in hit function to put into correct bin
    Matrix4x4 transform;
    Aabb aabb;
    std::vector<ByteBufferView> indices;
    std::vector<ByteBufferView> positions;
    std::vector<ByteBufferView> normals;

    MeshObject();

    // linearize this mesh into primitives we can sample
    std::vector<std::shared_ptr<Primitive>> toPrimitives();
};


#endif //BACHELORTHESIS_MESHOBJECT_H
