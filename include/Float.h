//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_FLOAT_H
#define BACHELORTHESIS_FLOAT_H

#include <ostream>

template<typename T>
struct Vec3 {
    T x;
    T y;
    T z;
public:
    Vec3() = default;

    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}

    Vec3(std::initializer_list<T> &list) {
        if (list.size() == 3) {
            x = list[0];
            y = list[1];
            z = list[2];
        }
    }

    friend std::ostream &operator<<(std::ostream &os, Vec3<T> v) {
        return os << "{" << v.x << ", " << v.y << ", " << v.z << "}";
    }
};

typedef float Float;
typedef Vec3<float> Float3;

#endif //BACHELORTHESIS_FLOAT_H
