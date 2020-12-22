//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_SCENE_H
#define BACHELORTHESIS_SCENE_H
#include <vector>
#include <string>
#include "Material.h"
#include "Matrix.h"


struct Buffer {
    std::string name;
    std::vector<unsigned char> data;
};


class Scene {
public:
    std::vector<int> objects;
    std::vector<Material> materials;
    void loadGLTF(const std::string& filename);

private:
    std::vector<Buffer> buffers;
};


#endif //BACHELORTHESIS_SCENE_H
