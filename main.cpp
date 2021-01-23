#include <iostream>
#include "Estimator.h"
#include "Scene.h"

int main() {
    Scene scene;
    scene.loadGLTF("planeInSphere.gltf");
    scene.buildSceneGeometry();
    Estimator estimator;
    estimator.scene = &scene;
    estimator.sampler.seed(1234);

    auto result = estimator.estimateAbsorption();
    std::stringstream stream;
    stream.width(20);
    stream.flags(std::ios::left);

    // header line
    stream << std::setw(20) << "Emitting Mesh";
    for (auto &o: scene.objects)
        stream << std::setw(20) << o.name;
    stream << std::endl;

    // one row for each mesh
    for (int i = 0; i < scene.nodeCount(); i++) {
        stream << std::setw(20) << scene.objects[i].name;
        for (int j = 0; j < scene.nodeCount(); j++)
            stream << std::setw(20) << result[i][j];
        stream << std::endl;
    }

    std::cout << stream.str();
}