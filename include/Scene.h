//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_SCENE_H
#define BACHELORTHESIS_SCENE_H
#include <vector>
#include <string>
#include "Buffer.h"
#include "Material.h"
#include "Matrix.h"
#include "MeshObject.h"

class Scene {
public:
    std::vector<MeshObject> objects;
    std::vector<Material> materials;
    void loadGLTF(const std::string& filename);

    std::vector<Buffer<u_char>> buffers;
};


#endif //BACHELORTHESIS_SCENE_H
