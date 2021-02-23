#include <iostream>
#include "Estimator.h"
#include "Scene.h"

int main() {
    for (std::string sceneName : {//"triangle",
                                "cube",
                                  //"planeInSphere",
                                  //"concentricSpheresSubdivided",
                                  //"ERNST_WITH_MATERIALS"

    }) {
        auto scene = std::make_shared<Scene>();
        scene->loadGLTF(sceneName + ".gltf");

        // add a default BB material if none was given
        if (!scene->materials.empty()) {
            scene->materials.clear();
            scene->materials.emplace_back("defaultBB");
            scene->materials.emplace_back("defaultSpec", 0, 1, 0);
        }
        scene->objects[0].materialIndex = 0;
        scene->objects[1].materialIndex = 0;
        scene->sampler->seed(12345);
        scene->buildSceneGeometry();

        scene->MeshToGnuPlotMesh();

        Estimator estimator(scene);

        auto result1 = estimator.estimateAbsorption();
        std::stringstream stream;
        stream.width(20);
        stream.flags(std::ios::left);


        stream << "result" << std::endl;
        // header line
        stream << std::setw(20) << "Emitting Mesh";
        for (auto &o: scene->objects)
            stream << std::setw(20) << o.name;
        stream << std::setw(20) << "Enclosure" << std::endl;

        // one row for each mesh
        for (int i = 0; i < scene->emittingNodeCount(); i++) {
            stream << std::setw(20) << scene->objects[i].name;
            for (int j = 0; j < scene->absorbingNodeCount(); j++)
                stream << std::setw(20) << result1[i][j];
            stream << std::endl;
        }
        std::cout << stream.str();
    }
}