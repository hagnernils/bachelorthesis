#include <iostream>
#include <fstream>
#include "Estimator.h"
#include "Scene.h"

int main(int argc, char* argv[]) {
    if (argc < 7)
        return 1;
    auto primsPerLeaf = argc > 1 ? std::strtol(argv[1], nullptr, 10) : 5;
    auto samplesPerTriangle = std::strtol(argv[2], nullptr, 10);
    auto traceDepth = std::strtol(argv[3], nullptr, 10);
    bool useMedianSplit = std::strtol(argv[4], nullptr, 10);
    bool useUniformSampling = std::strtol(argv[5], nullptr, 10);
    auto sceneName = std::string(argv[6]);
    auto outFileName = argc > 7 ? std::string(argv[7]) : "";

    auto scene = std::make_shared<Scene>();
    scene->loadGLTF(sceneName + ".gltf");

    scene->sampler->seed(12345);

    BVHNode::setConstructionOptions(primsPerLeaf, useMedianSplit ? SplitMethod::MEDIANCUT : SplitMethod::MIDPOINT);
    scene->buildSceneGeometry();


    Estimator estimator(scene);
    estimator.samplesPerPrimitive = samplesPerTriangle;
    estimator.traceDepth = traceDepth;
    estimator.useUniformSampling = useUniformSampling;

    auto result = estimator.estimateAbsorption();
    std::stringstream stream;
    stream.flags(std::ios::left);


    stream << "result for " << sceneName << " with rays" << std::endl;
    stream << samplesPerTriangle << std::endl;
    // header line
    auto width = 25;
    stream.width(width);
    stream << std::setw(width) << "Emitting Mesh";
    for (auto &o: scene->objects)
        stream << std::setw(width) << o.name;
    stream << std::setw(width) << "Enclosure" << std::endl;

    // one row for each mesh
    for (int i = 0; i < scene->emittingNodeCount(); i++) {
        stream << std::setw(width) << scene->objects[i].name;
        for (int j = 0; j < scene->absorbingNodeCount(); j++) {
            stream << std::setw(width) << std::setprecision(std::numeric_limits<Float>::max_digits10) << result[i][j];
        }
        stream << std::endl;
    }
    std::cout << stream.str();
    if (!outFileName.empty()) {
        std::ofstream outFileStream(outFileName, std::ios::app | std::ios::out);
        outFileStream << stream.str();
        // uncomment to write mesh visualization output, might be a large file
        //scene->MeshToGnuPlotMesh(result);
    }

}