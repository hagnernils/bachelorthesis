#include <iostream>
#include <fstream>
#include "Estimator.h"
#include "Scene.h"

int main(int argc, char* argv[]) {
    if (argc == 0)
        return 1;
    auto sceneNames = std::vector<std::string>{
            //"triangle",
            //"triangles",
            //"cube",
            //"planeInSphere",
            //"spheres_lowres",
            //"spheres_midres",
            //"spheres_highres",
            //"ERNST2019"
            //"ERNST_WITH_MATERIALS"
    };
    auto primsPerLeaf = argc > 1 ? std::strtol(argv[1], nullptr, 10) : 5;
    auto samplesPerTriangle = std::strtol(argv[2], nullptr, 10);
    auto traceDepth = std::strtol(argv[3], nullptr, 10);
    bool useMedianSplit = std::strtol(argv[4], nullptr, 10);
    auto sceneName = std::string(argv[5]);

    auto scene = std::make_shared<Scene>();
    scene->loadGLTF(sceneName + ".gltf");

    // add a default BB material if none was given
    if (!scene->materials.empty()) {
        scene->materials = {
            Material("DefaultBB"),
            Material("partSpec", 0.7, 0.3),
            Material("defaultSpec", 0.5, 0.5, 0),
        };
    }
    for (auto &obj : scene->objects)
        obj.materialIndex = 0;
    scene->sampler->seed(12345);

    BVHNode::setConstructionOptions(primsPerLeaf, useMedianSplit ? SplitMethod::MEDIANCUT : SplitMethod::MIDPOINT);
    scene->buildSceneGeometry();

    //scene->MeshToGnuPlotMesh();

    Estimator estimator(scene);
    estimator.samplesPerPrimitive = samplesPerTriangle;
    estimator.traceDepth = traceDepth;

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
    std::cerr << stream.str();
    std::ofstream outFileStream("output.txt");
    outFileStream << stream.str();
}