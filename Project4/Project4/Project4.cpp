#include <stdio.h>
#include <math.h>
#include <windows.h>

#define WIDTH 40
#define HEIGHT 20

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    float x, y, w;
} vec3;

typedef struct {
    float m[3][3];
} Matrix3x3;

// 3x3 행렬 초기화
void initIdentityMatrix(Matrix3x3* mat) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mat->m[i][j] = (i == j) ? 1 : 0;
        }
    }
}

// 벡터와 행렬 곱셈
vec3 Mul(vec3 a, Matrix3x3 b) {
    vec3 result;
    result.x = a.x * b.m[0][0] + a.y * b.m[0][1] + a.w * b.m[0][2];
    result.y = a.x * b.m[1][0] + a.y * b.m[1][1] + a.w * b.m[1][2];
    result.w = a.x * b.m[2][0] + a.y * b.m[2][1] + a.w * b.m[2][2];
    return result;
}

// 행렬과 행렬 곱셈
Matrix3x3 Mul(Matrix3x3 a, Matrix3x3 b) {
    Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j];
        }
    }
    return result;
}

// 이동 행렬 생성
Matrix3x3 createTranslationMatrix(float tx, float ty) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    mat.m[0][2] = tx;
    mat.m[1][2] = ty;
    return mat;
}

// 회전 행렬 생성
Matrix3x3 createRotationMatrix(float angle) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    float radian = angle * M_PI / 180;
    mat.m[0][0] = cos(radian);
    mat.m[0][1] = -sin(radian);
    mat.m[1][0] = sin(radian);
    mat.m[1][1] = cos(radian);
    return mat;
}

// 콘솔 화면 지우기
void clearScreen() {
    system("cls");
}

// 화면 그리기
void draw(int angleObj2, int angleObj3) {
    clearScreen();

    // 중심 객체 위치 (고정)
    int centerX = WIDTH / 2;
    int centerY = HEIGHT / 2;

    // 초기 위치 설정
    vec3 obj2 = { 0.0f, 5.0f, 1.0f }; // 중심에서 A까지 거리
    vec3 obj3 = { 0.0f, 2.0f, 1.0f }; // A에서 B까지 거리

    // 행렬 곱셈을 중첩된 형식으로 표현
    Matrix3x3 rotationObj2 = createRotationMatrix(angleObj2);   // A의 회전
    Matrix3x3 rotationObj3 = createRotationMatrix(angleObj3);   // B의 회전
    Matrix3x3 translateToCenter = createTranslationMatrix(centerX, centerY); // 중심으로의 이동
    Matrix3x3 translateObj2ToObj3 = createTranslationMatrix(obj2.x, obj2.y); // A를 기준으로 B로의 이동

    // e = Mul(A, Mul(B, Mul(C, D))) 형식으로 변환
    Matrix3x3 transformA = Mul(translateToCenter, rotationObj2);   // 중심에서 회전한 A의 변환
    Matrix3x3 transformB = Mul(transformA, translateObj2ToObj3);   // A를 기준으로 B의 변환
    Matrix3x3 finalTransformB = Mul(transformB, rotationObj3);     // B에 회전 변환을 적용하여 최종 위치

    // 변환 후 좌표 계산
    obj2 = Mul(obj2, transformA); // A의 최종 위치
    obj3 = Mul(obj3, finalTransformB); // B의 최종 위치

    // 화면에 그리기
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == centerX && y == centerY) {
                printf("C"); // 중심 객체
            }
            else if ((int)obj2.x == x && (int)obj2.y == y) {
                printf("A"); // 두 번째 객체
            }
            else if ((int)obj3.x == x && (int)obj3.y == y) {
                printf("B"); // 세 번째 객체
            }
            else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main() {
    int angleObj2 = 0;
    int angleObj3 = 0;

    while (1) {
        draw(angleObj2, angleObj3);

        // 각도 증가
        angleObj2 = (angleObj2 + 5) % 360;    // 두 번째 객체 회전 속도
        angleObj3 = (angleObj3 + 15) % 360;   // 세 번째 객체 회전 속도

        Sleep(100); // 0.1초 대기
    }
    return 0;
}