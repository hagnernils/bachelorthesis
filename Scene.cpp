//
// Created by Nils Hagner on 20.12.20.
//
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <vector>
#include <MeshObject.h>
#include <Spectrum.h>
#include "tinygltf/tiny_gltf.h"
#include "Scene.h"
#include "Material.h"
#include "Matrix.h"
#include "Buffer.h"
#include "Buffer.cpp"

template<typename T>
BufferView<T> AccessorToBufferView(Scene &scene, const int accessorIndex, const tinygltf::Model &model) {
    if (accessorIndex == -1) {
        std::cerr << "Invalid Accessor" << std::endl;
        return BufferView<T>();
    }
    auto accessor = model.accessors[accessorIndex];
    auto gltfBufferView = model.bufferViews[accessor.bufferView];
    T *bufferBegin = reinterpret_cast<T *>(scene.buffers[gltfBufferView.buffer].data.data());
    BufferView<T> bufferView(bufferBegin
                             + gltfBufferView.byteOffset
                             + accessor.byteOffset);
    bufferView.numElements = accessor.count;
    bufferView.byteStride = gltfBufferView.byteStride ? gltfBufferView.byteStride : sizeof(T);
    return bufferView;
}


void processNode(Scene &scene, size_t nodeIndex, const tinygltf::Model &model, const Matrix4x4 &parentTransform) {
    auto node = model.nodes[nodeIndex];

#ifdef VERBOSE
    std::cerr << "Parent transform is " << parentTransform;
    std::cerr << "node " << node.name << " with index " << nodeIndex << std::endl;
#endif
    Matrix4x4 translation, rotation, scale;


    translation = node.translation.empty()
                  ? Matrix4x4::identity()
                  : Matrix4x4::translate(node.translation[0],
                                         node.translation[1],
                                         node.translation[2]);

    rotation = node.rotation.empty()
               ? Matrix4x4::identity()
               : Matrix4x4::rotationMatrixFromQuaternion(node.rotation[0],
                                                         node.rotation[1],
                                                         node.rotation[2],
                                                         node.rotation[3]);

    scale = node.scale.empty()
            ? Matrix4x4::identity()
            : Matrix4x4::scale(node.scale[0],
                               node.scale[1],
                               node.scale[2]);

    std::vector<float> nodeMatrix;
    std::transform(node.matrix.begin(), node.matrix.end(), nodeMatrix.begin(),
                   [](double x) { return static_cast<float>(x); });

    Matrix4x4 matrix = node.matrix.empty()
                       ? Matrix4x4::identity()
                       : Matrix4x4(nodeMatrix.data()).transpose();

    Matrix4x4 fullTransform = parentTransform * matrix * translation * rotation * scale;

#ifdef VERBOSE
    std::cerr << "Full transform is " << fullTransform;
#endif

    // get indices, positions and normals of the mesh
    if (node.mesh != -1) {
        auto &mesh = model.meshes[node.mesh];
        if (mesh.primitives.size() > 1)
            std::cerr << "Warning: Mesh with name " << mesh.name << " has " << mesh.primitives.size() << " primitives" << std::endl;
        for (auto &primitive : mesh.primitives) {
#ifdef VERBOSE
            std::cerr << "has indices accessor " << primitive.indices << std::endl << std::endl;
#endif
            if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
                continue;

            MeshObject meshObject;
            meshObject.name = mesh.name;
            meshObject.materialIndex = primitive.material == -1 ? 0 : primitive.material;
            meshObject.transform = fullTransform;

            auto indicesBufferView = AccessorToBufferView<unsigned char>(scene, primitive.indices, model);
            meshObject.indices.push_back(indicesBufferView);

            auto positionAccessor = primitive.attributes.at("POSITION");
            auto positionBufferView = AccessorToBufferView<unsigned char>(scene, positionAccessor, model);
            meshObject.positions.push_back(positionBufferView);

            // construct AABB
            auto minValues = model.accessors[positionAccessor].minValues;
            auto maxValues = model.accessors[positionAccessor].maxValues;
            if (!minValues.empty() && !maxValues.empty())
                meshObject.aabb = Aabb{Float3(minValues[0], minValues[1], minValues[2]),
                                       Float3(maxValues[0], maxValues[1], maxValues[2])};


            auto normalsAccessor = primitive.attributes.at("NORMAL");
            auto normalsBufferView = AccessorToBufferView<unsigned char>(scene, normalsAccessor, model);
            meshObject.normals.push_back(normalsBufferView);

            assert(meshObject.positions.size() == meshObject.indices.size());

            scene.objects.push_back(meshObject);
        }
    }

    for (auto childIndex : node.children) {
        processNode(scene, childIndex, model, fullTransform);
    }
}

void Scene::loadGLTF(const std::string &filename) {
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string error, warning;

    // load model
    bool result = loader.LoadASCIIFromFile(&model, &error, &warning, filename);
    if (!result) {
        std::cerr << "Failed to load scene" << std::endl;
        std::cerr << error << std::endl;
        throw;
    }
    if (!warning.empty()) {
        std::cerr << "Warning while loading gltf: " << warning << std::endl;
    }

    // Save buffers to scene
    for (const auto &b : model.buffers) {
        buffers.emplace_back(b.name, b.data);
    }

    // Add materials
    for (const auto &material : model.materials) {
        std::cerr << "Found Material : " << material.name << std::endl;

        // the four-component base color factor is used for the material values
        const auto base_color_it = material.values.find("baseColorFactor");
        if (base_color_it != material.values.end()) {
            const tinygltf::ColorValue c = base_color_it->second.ColorFactor();
            materials.emplace_back(material.name, c[0], c[1], c[2]);
        } else {
            std::cerr << "Using default black body material" << std::endl;
        }
    }

    if (materials.empty())
        materials.emplace_back("defaultBB");

    // filter out nodes not used as child = root node(s)
    std::vector<bool> isRootNode(model.nodes.size(), true);
    for (auto &node : model.nodes)
        for (auto &child : node.children)
            isRootNode[child] = false;

    // compute the object->world transforms of the nodes
    for (int i = 0; i < isRootNode.size(); i++) {
        if (isRootNode[i]) {
            processNode(*this, i, model, Matrix<float, 4, 4>::identity());
        }
    }
}

bool Scene::closestHit(Ray r, HitRecord *hitRecord) {
    Float tMin = 0.001;
    Float tMax = 1e16f;
    bool hit = false;
#define EXHAUSTIVE_INTERSECTION
#ifndef EXHAUSTIVE_INTERSECTION
    hit = bvh.hit(r, tMin, tMax, hitRecord);
#else
    for (auto &p: sceneGeometry)
        if (p->hit(r, tMin, tMax, hitRecord)) {
            tMax = hitRecord->time;
            hit = true;
        }
#endif
    return hit;
}

void Scene::buildSceneGeometry() {
    unsigned int id = 0;
    unsigned int pid = 0;
    for (auto &object : objects) {
        object.objectID = id++;
        auto prims = object.toPrimitives();
        for (auto& prim : prims) {
            prim->Id = pid++;
        }
        primsOfObject.push_back(prims);
        sceneGeometry.insert(sceneGeometry.end(), prims.begin(), prims.end());
    }

    bvh = BVHNode(sceneGeometry, 0, sceneGeometry.size());
}

void Scene::MeshToGnuPlotMesh(const std::vector<AbsorbedEnergySpectrum> &result) {
    std::fstream outfileverts("meshVerts.txt", std::ios::out);
    std::fstream outfileindices("meshIndices.txt", std::ios::out);

    // calculate global view factor = sum of columns
    std::vector<double> globalVfs(result.size(), 0.);
    for (auto &absorbedEnergySpectrum : result) {
        for (int i = 0; i < absorbedEnergySpectrum.count - 1; i++)
            globalVfs[i] += absorbedEnergySpectrum[i];
    }

    std::vector<int> excludeObjectIds = { };

    auto sep = " ";
    size_t counter = 0;
    for (auto &p : sceneGeometry) {
        // TODO: this excludes 2 sides from the ERNST_truescale model. comment out if needed
        if (p->parent->name.rfind("Side_X-") == 0 || p->parent->name.rfind("Side_Y-") == 0)
            continue;
        if (std::find(excludeObjectIds.begin(), excludeObjectIds.end(), p->parent->objectID) != excludeObjectIds.end())
            continue;
        auto color = globalVfs[p->parent->objectID];
        outfileverts << p->a.toString() << " " << color << std::endl
                     << p->b.toString() << " " << color << std::endl
                     << p->c.toString() << " " << color << std::endl;

        outfileindices << ++counter << sep << ++counter << sep << ++counter << std::endl;
    }

}
