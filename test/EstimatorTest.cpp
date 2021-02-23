//
// Created by Nils Hagner on 17.01.21.
//

#include "gtest/gtest.h"
#include "Estimator.h"
#include "../Estimator.cpp"

TEST(Estimate, EstimatorRectangleToRectangle) {
    Scene scene;
    scene.loadGLTF("twoSquares.gltf");
    scene.buildSceneGeometry();
    Estimator estimator;
    estimator.scene = &scene;
    estimator.sampler.seed(1234);

    auto result = estimator.estimateAbsorption();

    for (auto &v : result) {
        std::cout << v << std::endl;
    }

}