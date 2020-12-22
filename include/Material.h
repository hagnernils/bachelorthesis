//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_MATERIAL_H
#define BACHELORTHESIS_MATERIAL_H
#include <string>

struct Material {
    std::string name;
    float absorption = 1.f; // equals emission according to Kirchhoffs law
    float specularReflection = 0.f;
    float diffuseReflection = 0.f;
};


#endif //BACHELORTHESIS_MATERIAL_H
