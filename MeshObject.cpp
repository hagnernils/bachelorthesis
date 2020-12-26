//
// Created by Nils Hagner on 20.12.20.
//

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include "MeshObject.h"

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
        const auto positionBuffer = positions[subMeshIndex].data;
        const auto indexBuffer = indices[subMeshIndex].data;

        std::vector<uint16_t> indicesVector;
        for (unsigned int i = 0; i < indices[subMeshIndex].numElements; i++) {
            indicesVector.push_back(indexBuffer[i]);
        }

        std::vector<Float3> vertexPositionsVector;
        float temp[3];
        for (unsigned int i = 0; i < indicesVector.size(); i++) {
            std::memcpy(temp, &positionBuffer[i], sizeof(float) * 3);
            vertexPositionsVector.emplace_back(temp[0], temp[1], temp[2]);
        }

        const size_t numIndexTriplets = indices[subMeshIndex].numElements / 3;

        for (unsigned int i = 0; i < numIndexTriplets; i++) {
            prims.emplace_back(vertexPositionsVector[i + 0],
                               vertexPositionsVector[i + 1],
                               vertexPositionsVector[i + 2],
                               Float3{},
                               std::make_shared<MeshObject>(*this));
        }
    }

    return prims;
}
