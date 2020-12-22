//
// Created by Nils Hagner on 22.12.20.
//
#include "gtest/gtest.h"
#include "Matrix.h"

TEST (Matrix, MatrixMultiplicationIdempotence) {
    Matrix4x4 A = Matrix4x4::identity();
    Matrix4x4 B = Matrix4x4::identity();
    EXPECT_EQ (A, A*A);
    EXPECT_EQ (A, A*B);
    EXPECT_EQ (A, B*A);

}
TEST (Matrix, MatrixMultiplicationIdentityElement) {
    Matrix4x4 A = Matrix4x4::identity();
    Matrix4x4 B = Matrix4x4::scale(1, 2, 3);
    Matrix4x4 C = Matrix4x4::translate(1, 2, 3);
    Matrix4x4 D = Matrix4x4::rotationMatrixFromQuaternion(1, 2, 3, 4);


    EXPECT_EQ(B, A*B);
    EXPECT_EQ(B, A*B);

    EXPECT_EQ(C, A*C);
    EXPECT_EQ(C, C*A);

    EXPECT_EQ(D, A*D);
    EXPECT_EQ(D, D*A);

}

TEST (Matrix, MatrixMultiplicationNonCommutativity) {
    Matrix4x4 A = Matrix4x4::scale(1, 2, 3);
    Matrix4x4 B = Matrix4x4::translate(1, 2, 3);

    EXPECT_NE(B*A, A*B);
}
