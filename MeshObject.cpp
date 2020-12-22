//
// Created by Nils Hagner on 20.12.20.
//

#include "include/MeshObject.h"

MeshObject::MeshObject() {
    name = "MeshObject";
    materialIndex = 0;
}

MeshObject::MeshObject(std::string name, size_t materialIndex) {
    name = name;
    materialIndex = materialIndex;
}
