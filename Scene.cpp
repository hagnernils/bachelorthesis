//
// Created by Nils Hagner on 20.12.20.
//
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <vector>
#include <MeshObject.h>
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
    bufferView.elementSizeInBytes = accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT ? 2 :
                                    accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT ? 4 :
                                    accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT ? 4 :
                                    sizeof(T);
    return bufferView;
}


void processNode(Scene &scene, size_t nodeIndex, const tinygltf::Model &model, const Matrix4x4 &parentTransform) {
    std::cerr << "Parent transform is " << parentTransform;

    auto node = model.nodes[nodeIndex];

    std::cerr << "node " << node.name << " with index " << nodeIndex << std::endl;

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

    std::cerr << "Full transform is " << fullTransform;

    // get indices, positions and normals of the mesh
    if (node.mesh != -1) {
        auto &mesh = model.meshes[node.mesh];
        std::cerr << "Mesh with name " << mesh.name << std::endl;
        for (auto &primitive : mesh.primitives) {
            std::cerr << "has indices accessor " << primitive.indices << std::endl << std::endl;

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
        Material mat = Material(material.name);

        // the four-component base color factor is used for the material values
        const auto base_color_it = material.values.find("baseColorFactor");
        if (base_color_it != material.values.end()) {
            const tinygltf::ColorValue c = base_color_it->second.ColorFactor();
            mat.absorption = c[0];
            mat.specularReflection = c[1];
            mat.diffuseReflection = c[2];
        } else {
            std::cerr << "Using default black body material" << std::endl;
        }

        std::cerr << "Material " << mat.name << ":" << std::endl
                  << "Absorption " << mat.absorption
                  << " Specular Reflection " << mat.specularReflection
                  << " Diffuse Reflection " << mat.diffuseReflection
                  << std::endl;

        materials.push_back(mat);
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
    Float tMin = 0;
    Float tMax = 1e16f;
    bool hit = false;
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
    size_t offset = 0;
    hitBVH = std::vector<LinearBVHNode>(2 << (bvh.depth() + 1));
    bvh.linearize(hitBVH, offset);
}

void Scene::MeshToGnuPlotMesh(const std::string& filename) {
    std::fstream outfile("mesh.out", std::ios::out);
    auto len = sceneGeometry.size();
    std::vector<Float> x, y, z;
    std::vector<size_t> tris;
    for (const auto& p: sceneGeometry) {
        x.push_back(p->a.x);
        y.push_back(p->a.y);
        z.push_back(p->a.z);

        x.push_back(p->b.x);
        y.push_back(p->b.y);
        z.push_back(p->b.z);

        x.push_back(p->c.x);
        y.push_back(p->c.y);
        z.push_back(p->c.z);

        size_t vI = tris.size();
        tris.insert(tris.end(), {vI, vI + 1, vI + 2});
    }
    auto sep = ", ";

    for (auto &vec : {x, y, z}) {
        for (auto val : vec) {
            outfile << val << sep;
        }
        outfile << std::endl;
    }

    for (auto i = 0; i < len; i++)
        outfile << tris[3 * i] << sep << tris[3 * i + 1] << sep << tris[3 * i + 2] << std::endl;

}
