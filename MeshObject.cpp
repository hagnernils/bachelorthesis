//
// Created by Nils Hagner on 20.12.20.
//

#include <cassert>
#include <cstring>
#include <iostream>
#include "MeshObject.h"
#include "Buffer.cpp"
#include "Primitive.h"


MeshObject::MeshObject() {
    name = "MeshObject";
    materialIndex = 0;
    aabb = Aabb{Float3{0, 0, 0},
                Float3{0, 0, 0}};
}

std::vector<Primitive> MeshObject::toPrimitives() {

    auto prims = std::vector<Primitive>();

    for (unsigned int subMeshIndex = 0; subMeshIndex < indices.size(); subMeshIndex++) {
        // we only want valid meshes with triangles
        assert(indices[subMeshIndex].numElements % 3 == 0);
        const auto positionBuffer = positions[subMeshIndex];
        const auto indexBuffer = indices[subMeshIndex];

        std::vector<Float3> vertexPositionsVector;
        float temp[3];
        for (size_t i = 0; i < indexBuffer.numElements; i++) {
            uint16_t index = *reinterpret_cast<uint16_t *>(indexBuffer.data + i * sizeof(uint16_t));
            std::memcpy(temp, positionBuffer.data + index * 3 * sizeof(float), sizeof(float) * 3);
            vertexPositionsVector.emplace_back(temp[0], temp[1], temp[2]);
        }


        const size_t numIndexTriplets = indexBuffer.numElements / 3;

        for (unsigned int i = 0; i < numIndexTriplets; i++) {
            prims.emplace_back(vertexPositionsVector[3 * i + 0],
                               vertexPositionsVector[3 * i + 1],
                               vertexPositionsVector[3 * i + 2],
                               std::make_shared<MeshObject>(*this));
        }
    }

    return prims;
}
