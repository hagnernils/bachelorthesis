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
typedef Vector<float, 3> Float3;

template<typename T>
struct Vector<T, 3> {
    union {
        std::array<T, 3> data;
        struct { T x, y, z; };
    };

    Vector() = default;
    ~Vector() = default;

    Vector(const Vector&& v)  noexcept : data(std::move(v.data)) {}

    explicit Vector(const Vector<T, 3> &v) : data(v.data) {}

    Vector<T, 3>(T x, T y, T z) : data({x,y,z}) {}

    Vector(std::initializer_list<T> list) {
        std::copy(list.begin(), list.end(), data.begin());
    }

    inline T dot(const Vector<T, 3> &v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    inline Vector<T, 3> &normalize() {
        return operator/=(length());
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
        data[0] = other.data[0];
        data[1] = other.data[1];
        data[2] = other.data[2];
        return *this;
    }


    inline Vector<T, 3> &operator-=(const Vector<T, 3> &v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    inline Vector<T, 3> &operator+=(const Vector<T, 3> &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    inline Vector<T, 3> &operator*=(const T &factor) {
        x *= factor;
        y *= factor;
        z *= factor;
        return *this;
    }

    inline Vector<T, 3> &operator/=(const T &divisor) {
        x /= divisor;
        y /= divisor;
        z /= divisor;
        return *this;
    }

    bool operator==(const Vector<T, 3> &v) const {
        return x == v.x
               && y == v.y
               && z == v.z;
    }

    bool operator!=(const Vector<T, 3> &v) const {
        return x != v.x
               || y != v.y
               || z != v.z;
    }
};

template<typename T>
inline Vector<T, 3> operator-(Vector<T, 3> lhs, const Vector<T, 3> &rhs) {
    lhs -= rhs; return lhs;
}

template<typename T>
inline Vector<T, 3> operator+(Vector<T, 3> lhs, const Vector<T, 3> &rhs) {
    lhs += rhs; return lhs;
}

template<typename T1, typename T2>
inline Vector<T2, 3> operator*(Vector<T2, 3> v, const T1 factor) {
    v *= factor; return v;
}

template<typename T1, typename T2>
inline Vector<T2, 3> operator*(const T1 factor, const Vector<T2, 3> &v) {
    return v * factor;
}

template<typename T>
inline Vector<T, 3> operator/(Vector<T, 3> v, const T divisor) {
    v /= divisor; return v;
}

template<typename T>
inline Vector<T, 3> operator/(const T divisor, Vector<T, 3> v) {
    v /= divisor; return v;
}

#endif //BACHELORTHESIS_FLOAT_H
