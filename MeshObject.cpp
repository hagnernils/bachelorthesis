//
// Created by Nils Hagner on 20.12.20.
//

#include "include/MeshObject.h"

MeshObject::MeshObject() {
    name = "MeshObject";
    materialIndex = 0;
    aabb = Aabb{Float3{0,0,0},
                Float3{0,0,0}};
}