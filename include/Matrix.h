//
// Created by Nils Hagner on 21.12.20.
//

#ifndef BACHELORTHESIS_MATRIX_H
#define BACHELORTHESIS_MATRIX_H
#include <algorithm>
#include <functional>
#include <ostream>



template <typename T, unsigned int M, unsigned int N>
class Matrix {
private:
    T data[M * N];

public:
    Matrix();
    explicit Matrix(const float other[M * N]) { for(unsigned int i = 0; i < M * N; i++) data[i] = other[i];}
    explicit Matrix(const T val);
    Matrix(const std::initializer_list<T>& list);

    T operator[](unsigned int i) const { return data[i]; }
    T &operator[](unsigned int i) { return data[i]; }
    Matrix& operator=(const Matrix& other);
    Matrix<T, M, N>& operator+=(const Matrix<T, M, N> &other);
    Matrix<T, M, N>& operator-=(const Matrix<T, M, N> &other);

    Matrix<T, N, M> transpose();

    static Matrix<T, N, N> identity();
    // create an Homogenous translation matrix
    static Matrix<T, 4, 4> translate(const T x, const T y, const T z);
    static Matrix<T, 4, 4> scale(const T x, const T y, const T z);
    static Matrix<T, 4, 4> rotationMatrixFromQuaternion(const float qw,
                                                        const float qx,
                                                        const float qy,
                                                        const float qz);
    void printTo(std::ostream &stream);
    T* getData() { return data; };
};

typedef Matrix<float, 4, 4> Matrix4x4;


template<typename T, unsigned int M, unsigned int N, unsigned int A>
Matrix<T, M, N> operator*(const Matrix<T,M,A> &lhs, const Matrix<T,A,N> &rhs) {
    T temp[M * N];
    for (unsigned int i = 0; i < M; i++) {
        for (unsigned int j = 0; j < N; j++) {
            T sum = 0;
            for (unsigned int k = 0; k < A; k++) {
                sum += lhs[i * N + k] * rhs[k * A + j];
            }
            temp[i * A + j] = sum;
        }
    }
    return Matrix4x4(temp);
}

template<typename T, unsigned int M, unsigned int N>
Matrix<T, M, N> &Matrix<T, M, N>::operator=(const Matrix<T, M, N> &other) {
    if (this != &other) {
        for (unsigned int i = 0; i < M * N; i++)
            data[i] = other[i];
    }
    return *this;
}

template<typename T, unsigned int M, unsigned int N>
Matrix<T, N, N> Matrix<T, M, N>::identity() {
    T temp[N * N];
    for (unsigned int i = 0; i < N * N; i++) {
        temp[i] = 0;
    }
    for (unsigned int i = 0; i < N; i++) {
        temp[N * i + i] = 1;
    }
    return Matrix<T, N, N>(temp);
}

template<typename T, unsigned int M, unsigned int N>
Matrix<T, M, N>::Matrix(const std::initializer_list<T> &list) {
    std::transform(list.begin(), list.end(), data, [](T t){return t;});
}

template<typename T, unsigned int M, unsigned int N>
void Matrix<T, M, N>::printTo(std::ostream &stream) {

    stream << "{ ";
    for (unsigned int i = 0; i < M * N; i++)
    {
        stream << data[i] << " ";
    }
    stream << " }" << std::endl;
}

template<typename T, unsigned int M, unsigned int N>
Matrix<T, 4, 4> Matrix<T, M, N>::translate(const T x, const T y, const T z) {
    return Matrix({1, 0, 0, x,
                   0, 1, 0, y,
                   0, 0, 1, z,
                   0, 0, 0, 1});
}

template<typename T, unsigned int M, unsigned int N>
Matrix<T, 4, 4> Matrix<T, M, N>::scale(const T x, const T y, const T z) {
    return Matrix({x, 0, 0, 0,
                   0, y, 0, 0,
                   0, 0, z, 0,
                   0, 0, 0, 1});
}


// TODO: find code or license for this
template<typename T, unsigned int M, unsigned int N>
Matrix<T, 4, 4>
Matrix<T, M, N>::rotationMatrixFromQuaternion(const float qw, const float qx, const float qy, const float qz) {
    T m[M*N];

    m[0*4+0] = 1.0f - 2.0f*qy*qy - 2.0f*qz*qz;
    m[0*4+1] = 2.0f*qx*qy - 2.0f*qz*qw;
    m[0*4+2] = 2.0f*qx*qz + 2.0f*qy*qw;
    m[0*4+3] = 0.0f;

    m[1*4+0] = 2.0f*qx*qy + 2.0f*qz*qw;
    m[1*4+1] = 1.0f - 2.0f*qx*qx - 2.0f*qz*qz;
    m[1*4+2] = 2.0f*qy*qz - 2.0f*qx*qw;
    m[1*4+3] = 0.0f;

    m[2*4+0] = 2.0f*qx*qz - 2.0f*qy*qw;
    m[2*4+1] = 2.0f*qy*qz + 2.0f*qx*qw;
    m[2*4+2] = 1.0f - 2.0f*qx*qx - 2.0f*qy*qy;
    m[2*4+3] = 0.0f;

    m[3*4+0] = 0.0f;
    m[3*4+1] = 0.0f;
    m[3*4+2] = 0.0f;
    m[3*4+3] = 1.0f;

    return Matrix4x4(m);
}

template<typename T, unsigned int M, unsigned int N>
Matrix<T, N, M> Matrix<T, M, N>::transpose() {
    Matrix<T, N, M> result;
    for (unsigned int i = 0; i< M; i++)
        for (unsigned int j = 0; j< N; j++)
            result[j * M + i] = data[i * N +j];
    return result;
}

template<typename T, unsigned int M, unsigned int N>
Matrix<T, M, N>::Matrix() = default;

template<typename T, unsigned int M, unsigned int N>
Matrix<T, M, N>::Matrix(const T val) {
    std::for_each(data, data + N * M - 1, [val](){ return val; });
}

template<typename T, unsigned int M, unsigned int N>
Matrix<T, M, N> &Matrix<T, M, N>::operator+=(const Matrix<T, M, N> &other) {
    std::transform(data, data + N * M - 1, other.getData(), std::plus<T>{});
    return *this;
}

template<typename T, unsigned int M, unsigned int N>
Matrix<T, M, N> &Matrix<T, M, N>::operator-=(const Matrix<T, M, N> &other) {
    std::transform(data, data + N * M - 1, other.getData(), std::minus<T>{});
    return *this;
}

template<typename T, unsigned int M, unsigned int N>
Matrix<T, M, N>& operator+(Matrix<T, M, N> &lhs, const Matrix<T, M, N> &rhs) {
    lhs += rhs;
    return lhs;
}

template<typename T, unsigned int M, unsigned int N>
Matrix<T, M, N>& operator-(Matrix<T, M, N> &lhs, const Matrix<T, M, N> &rhs) {
    lhs -= rhs;
    return lhs;
}

template<typename T, unsigned int M, unsigned int N>
bool operator==(const Matrix<T, M, N> &lhs, const Matrix<T, M, N> &rhs) {
    for (unsigned int i = 0; i < N * M; i++)
    {
        if (lhs[i] != rhs[i])
            return false;
    }
    return true;
}

template<typename T, unsigned int M, unsigned int N>
bool operator!=(const Matrix<T, M, N> &lhs, const Matrix<T, M, N> &rhs) {
    return !(lhs == rhs);
}
#endif //BACHELORTHESIS_MATRIX_H
