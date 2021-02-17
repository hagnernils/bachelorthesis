//
// Created by Nils Hagner on 20.12.20.
//

#include "include/Material.h"

Material::Material(std::string name, float absorption, float specularReflection, float diffuseReflection)
        : name(std::move(name)), absorption(absorption), specularReflection(specularReflection),
          diffuseReflection(diffuseReflection) {

    auto sum = absorption + specularReflection + diffuseReflection;
    cdf [0] = absorption / sum;
    cdf[1] = cdf[0] + specularReflection / sum;
    cdf[2] = cdf[1] + diffuseReflection / sum;
}

RaySurfaceInteraction Material::interact(float rand) {

    for (int i = 0; i < 3; i++) {
        if (cdf[i] >= rand) {
            return static_cast<RaySurfaceInteraction>(i);
        }
    }
}
