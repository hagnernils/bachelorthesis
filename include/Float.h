//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_FLOAT_H
#define BACHELORTHESIS_FLOAT_H

template <typename T>
class Vec3 {
    T x;
    T y;
    T z;
public:
    Vec3() = default;
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}
    Vec3(std::initializer_list<T> & list) {
        if (list.size() == 3) {
            x = list[0];
            y = list[1];
            z = list[2];
        }
    }
};
typedef float Float;
typedef Vec3<float> Float3;

#endif //BACHELORTHESIS_FLOAT_H
