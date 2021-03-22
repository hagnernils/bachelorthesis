//
// Created by Nils Hagner on 20.12.20.
//

#ifndef BACHELORTHESIS_FLOAT_H
#define BACHELORTHESIS_FLOAT_H

#include <iomanip>
#include <ostream>
#include <cmath>


template <typename T, unsigned int n>
struct Vector {
    T data[n];
};

typedef float Float;

typedef Vector<Float, 3> Vec3;

template<typename T>
struct Vector<T, 3> {
    union {
        T data[3];
        struct { T x, y, z; };
    };

    Vector() = default;

    constexpr Vector(const Vector& v) : x(v.x), y(v.y), z(v.z) {}

    constexpr Vector(const Vector&& v)  noexcept : x(v.x), y(v.y), z(v.z) {}

    Vector(T a, T b, T c) : x(a), y(b), z(c) {}

    Vector(std::initializer_list<T> &list) {
        auto i = 0;
        for (auto val : list)
            data[i++] = val;
    }

    inline T dot(const Vector<T, 3> &v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    inline Vector<T, 3> normalize() {
        T length = this->length();
        y /= length;
        z /= length;
        x /= length;
        return *this;
    }

    inline static Vector<T, 3> cross(const Vector<T, 3> &a, const Vector<T, 3> &b) {
        return Vec3(a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x);
    }

    inline static T length(const Vector<T, 3> &v) {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    inline T length() {
        return std::sqrt(x * x + y * y + z * z);
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector<T, 3> v) {
        os << std::setprecision(8) << std::fixed << std::showpoint;
        return os << "Float3 {" << v.x << ", " << v.y << ", " << v.z << "}";
    }

    std::string toString() {
        std::stringstream ss;
        ss << std::setprecision(8) << std::fixed << std::showpoint;
        ss << x << " " << y << " " << z;
        return ss.str();
    }

    inline T &operator[](size_t index) {
        return data[index];
    }

    inline T operator[](size_t index) const {
        return data[index];
    }

    inline Vector<T, 3> &operator-() {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    constexpr inline Vector<T, 3> &operator=(const Vector<T, 3> &other) {
        if (this == &other)
            return *this;
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }


    inline void operator-=(const Vector<T, 3> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    inline void operator+=(const Vector<T, 3> &v) {
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

    inline bool operator==(const Vector<T, 3> &v) const {
        return x == v.x
               && y == v.y
               && z == v.z;
    }

    inline bool operator!=(const Vector<T, 3> &v) const {
        return x != v.x
               || y != v.y
               || z != v.z;
    }
};

typedef Vector<float, 3> Float3;


template<typename T>
constexpr inline Vector<T, 3> operator-(const Vector<T, 3> &lhs, const Vector<T, 3> &rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

template<typename T>
constexpr inline Vector<T, 3> operator+(const Vector<T, 3> &lhs, const Vector<T, 3> &rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

template<typename T1, typename T2>
inline Vector<T2, 3> operator*(const Vector<T2, 3> &v, const T1 factor) {
    return Vector<T2, 3>(v.x * factor, v.y * factor, v.z * factor);
}

template<typename T1, typename T2>
inline Vector<T2, 3> operator*(const T1 factor, const Vector<T2, 3> &v) {
    return v * factor;
}

template<typename T>
inline T operator*(const Vector<T, 3> &lhs, const Vector<T, 3> &rhs) {
    return lhs.dot(rhs);
}

template<typename T>
inline Vector<T, 3> operator/(const Vector<T, 3> &v, const T divisor) {
    return Vector<T, 3>(v.x / divisor,
                v.y / divisor,
                v.z / divisor);
}

template<typename T>
inline Vector<T, 3> normalize(Vector<T, 3> &vec) {
    T length = vec.length();
    vec.y /= length;
    vec.z /= length;
    vec.x /= length;
    return vec;
}

#endif //BACHELORTHESIS_FLOAT_H
