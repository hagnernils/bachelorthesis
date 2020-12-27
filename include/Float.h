//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_FLOAT_H
#define BACHELORTHESIS_FLOAT_H

#include <iomanip>
#include <ostream>
#include <cmath>

template<typename T>
struct Vec3 {
    T x;
    T y;
    T z;

    Vec3() = default;

    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    Vec3(std::initializer_list<T> &list) {
        if (list.size() == 3) {
            x = list[0];
            y = list[1];
            z = list[2];
        }
    }

    inline T dot(const Vec3<T> &v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    inline void normalize() {
        T length = Vec3<T>::length(*this);
        y /= length;
        z /= length;
        x /= length;
    }

    inline static Vec3<T> cross(const Vec3<T> &a, const Vec3<T> &b) {
        return Vec3(a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x);
    }

    inline static T length(const Vec3<T> &v) {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    friend std::ostream &operator<<(std::ostream &os, const Vec3<T> v) {
        os << std::setprecision(8) << std::fixed << std::showpoint;
        return os << "Float3 {" << v.x << ", " << v.y << ", " << v.z << "}";
    }

    inline void operator-() {
        y = -y;
        z = -z;
        x = -x;
    }

    inline void operator-=(const Vec3<T> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    inline void operator+=(const Vec3<T> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    inline void operator*=(const T &factor) {
        x *= factor;
        y *= factor;
        z *= factor;
    }

    inline void operator/=(const T &divisor) {
        x /= divisor;
        y /= divisor;
        z /= divisor;
    }

    inline bool operator==(const Vec3<T> &v) const {
        return x == v.x
               && y == v.y
               && z == v.z;
    }

    inline bool operator!=(const Vec3<T> &v) const {
        return x != v.x
               || y != v.y
               || z != v.z;
    }
};

typedef float Float;
typedef Vec3<float> Float3;

template<typename T>
inline Vec3<T> operator-(const Vec3<T> &lhs, const Vec3<T> &rhs) {
    return Vec3(lhs.x - rhs.x,
                lhs.y - rhs.y,
                lhs.z - rhs.z);
}

template<typename T>
inline Vec3<T> operator+(const Vec3<T> &lhs, const Vec3<T> &rhs) {
    return Vec3(lhs.x + rhs.x,
                lhs.y + rhs.y,
                lhs.z + rhs.z);
}

template<typename T1, typename T2>
inline Vec3<T2> operator*(const Vec3<T2> &v, const T1 factor) {
    return Vec3<T2>(v.x * factor, v.y * factor, v.z * factor);
}

template<typename T1, typename T2>
inline Vec3<T2> operator*(const T1 factor, const Vec3<T2> &v) {
    return v * factor;
}

template<typename T>
inline T operator*(const Vec3<T> &lhs, const Vec3<T> &rhs) {
    return lhs.dot(rhs);
}

template<typename T>
inline Vec3<T> operator/(const Vec3<T> &v, const T divisor) {
    return Vec3(v.x / divisor,
                v.y / divisor,
                v.z / divisor);
}

#endif //BACHELORTHESIS_FLOAT_H
