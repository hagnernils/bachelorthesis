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
#include "Ray.h"
#include "HitRecord.h"
#include "Primitive.h"

class Scene {
public:
    std::vector<MeshObject> objects;
    std::vector<Material> materials;
    std::vector<Buffer<u_char>> buffers;

    size_t nodeCount() const { return objects.size(); }

    void loadGLTF(const std::string &filename);

    // prepare the scene so rays can be traced.
    void buildSceneGeometry();

    // check if a ray hits any object in the scene.
    bool closestHit(Ray r, HitRecord *hitRecord);

private:
    std::vector<Primitive> sceneGeometry;
};


#endif //BACHELORTHESIS_SCENE_H
