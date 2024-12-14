#ifndef ELFMATH_H
#define ELFMATH_H

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    float x, y;
} Vector2;

typedef struct {
    float m[3][3];
} Matrix3x3;

Matrix3x3 identity_matrix();
Matrix3x3 translation_matrix(float dx, float dy);
Matrix3x3 rotation_matrix(float theta);
Matrix3x3 scale_matrix(float sx, float sy);
Matrix3x3 multiply_matrices(Matrix3x3 a, Matrix3x3 b);
Vector3 multiply_matrix_vector(Matrix3x3 mat, Vector3 v);
Vector2 multiply_matrix_vector2(Matrix3x3 mat, Vector2 v);

#endif
