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

template <typename T>
BufferView<T> AccessorToBufferView(Scene& scene, const int accessorIndex, const tinygltf::Model& model) {
    BufferView<T> bufferView;
    if (accessorIndex == -1) {
        std::cerr << "Invalid Accessor" << std::endl;
        return bufferView;
    }
    auto accessor = model.accessors[accessorIndex];
    auto gltfBufferView = model.bufferViews[accessor.bufferView];
    bufferView.data = scene.buffers[gltfBufferView.buffer].data.begin();
    bufferView.data += gltfBufferView.byteOffset;
    bufferView.byteStride = gltfBufferView.byteStride;
    bufferView.numElements = 0;
    bufferView.elementSizeInBytes = sizeof(T);


    bufferView.data += accessor.byteOffset;
    bufferView.numElements = accessor.count;
    bufferView.elementSizeInBytes = accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT ? 2 :
                                    accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT   ? 4 :
                                    accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT          ? 4 :
                                    0;
    return bufferView;
}



void processNode(Scene &scene, size_t nodeIndex, const tinygltf::Model &model, Matrix4x4 parentTransform) {
    parentTransform.printTo(std::cerr);

    auto node = model.nodes[nodeIndex];

    std::cerr << "node " << node.name << " with index " << nodeIndex << std::endl;

    Matrix4x4 translation = Matrix4x4::identity(),
            rotation = Matrix4x4::identity(),
            scale = Matrix4x4::identity();


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
                   [](double x){return static_cast<float>(x);});

    Matrix4x4 matrix = node.matrix.empty()
            ? Matrix4x4::identity()
            : Matrix4x4(nodeMatrix.data()).transpose();

    translation.printTo(std::cerr);
    rotation.printTo(std::cerr);
    scale.printTo(std::cerr);
    matrix.printTo(std::cerr);

    Matrix4x4 fullTransform = parentTransform * matrix * translation * rotation * scale;

    fullTransform.printTo(std::cerr);

    // get indices, positions and normals of the mesh
    if (node.mesh != -1) {
        auto& mesh = model.meshes[node.mesh];
        std::cerr << "Mesh with name " << mesh.name << std::endl;
        for (auto& primitive : mesh.primitives) {
            std::cerr << "has indices accessor " << primitive.indices << std::endl << std::endl;

            if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
                continue;

            MeshObject meshObject;
            meshObject.name = mesh.name;
            meshObject.materialIndex = primitive.material;
            meshObject.transform = fullTransform;

            auto indicesBufferView = AccessorToBufferView<u_char>(scene, primitive.indices, model);
            meshObject.indices.push_back(indicesBufferView);

            auto positionAccessor = primitive.attributes.at("POSITION");
            auto positionBufferView = AccessorToBufferView<u_char>(scene, positionAccessor, model);
            meshObject.positions.push_back(positionBufferView);

            auto normalsAccessor = primitive.attributes.at("NORMAL");
            auto normalsBufferView = AccessorToBufferView<u_char>(scene, normalsAccessor, model);
            meshObject.normals.push_back(normalsBufferView);

            scene.objects.push_back(meshObject);
        }
    }

    for (auto childIndex : node.children) {
        processNode(scene, childIndex, model, fullTransform);
    }
}

void Scene::loadGLTF(const std::string& filename) {
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string error, warning;

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
        buffers.emplace_back(Buffer<u_char>{b.name, b.data});
    }

    // Add materials
    for (const auto &material : model.materials) {
        std::cerr << "Found Material : " << material.name << std::endl;
        Material mat;
        mat.name = material.name;

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

