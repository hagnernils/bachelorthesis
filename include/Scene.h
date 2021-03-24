//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_SCENE_H
#define BACHELORTHESIS_SCENE_H

#include <utility>
#include <vector>
#include <string>
#include "Buffer.h"
#include "BVH.h"
#include "Material.h"
#include "Matrix.h"
#include "MeshObject.h"
#include "Ray.h"
#include "HitRecord.h"
#include "Primitive.h"
#include "gtest/gtest.h"

class Scene {
public:
    std::vector<MeshObject> objects;
    std::vector<Material> materials = { Material{"default", 1,0,0}};
    std::vector<Buffer<u_char>> buffers;
    std::shared_ptr<DefaultSampler> sampler = std::make_shared<DefaultSampler>();
    std::vector<std::vector<std::shared_ptr<Primitive>>> primsOfObject;


    Scene() {
        sampler->seed(12345);
        bvh.setSampler(sampler);
    }

    // Node count, exclusive the Miss / Environment / Enclosure
    size_t emittingNodeCount() const { return objects.size(); }

    size_t absorbingNodeCount() const { return objects.size() + 1; }

    void loadGLTF(const std::string &filename);

    void MeshToGnuPlotMesh(const std::string& filename="mesh.out.txt");

    // prepare the scene so rays can be traced.
    void buildSceneGeometry();

    // check if a ray hits any object in the scene.
    bool closestHit(Ray r, HitRecord *hitRecord);

private:
    FRIEND_TEST(Scene, BuildBVH);
    std::vector<std::shared_ptr<Primitive>> sceneGeometry;
    std::vector<LinearBVHNode> hitBVH;
    BVHNode bvh;
};


#endif //BACHELORTHESIS_SCENE_H
