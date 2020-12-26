#include <iostream>
#include "Scene.h"

int main() {
    Scene scene;
    scene.loadGLTF("ERNST_WITH_MATERIALS.gltf");
    auto prims = scene.objects[0].toPrimitives();

    for (auto &prim : prims) {
        std::cerr << prim << std::endl;
    }
}