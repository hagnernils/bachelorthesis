//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_MATERIAL_H
#define BACHELORTHESIS_MATERIAL_H
#include <string>
#include "HitRecord.h"

enum RaySurfaceInteraction {
    ABSORPTION = 0,
    SPECULAR_REFLECTION = 1,
    DIFFUSE_REFLECTION = 2,
};

struct Material {
    std::string name;
    float absorption = 1.f; // equals emission according to Kirchhoffs law
    float specularReflection = 0.f;
    float diffuseReflection = 0.f;

    explicit Material(std::string name, float absorption = 1, float specularReflection = 0, float diffuseReflection = 0);

    RaySurfaceInteraction interact(float rand);

private:
    float cdf[3] = {1, 0, 0};
};


#endif //BACHELORTHESIS_MATERIAL_H
