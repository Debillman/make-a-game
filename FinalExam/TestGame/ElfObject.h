#ifndef ELFOBJECT_H  // 정의되어 있지 않으면
#define ELFOBJECT_H  // 정의하고 아래 코드를 컴파일

#include "ElfMath.h"


//게임 오브젝트
typedef struct
{
    //Transform
    Vector2 Position;
    float Rotation;
    Vector2 Scale;

    //Geometry
    Vector2 Line[2];

    //픽셀채울 글자
    char* Symbol;

}GameObject_Line;

typedef struct {
    float x; // x 좌표
    float y; // y 좌표
} Point;

// 선분(Line)을 정의: 두 점으로 구성
typedef struct {
    Point start; // 선분의 시작점
    Point end;   // 선분의 끝점
} Line;

// 삼각형(Triangle)을 정의: 세 점과 세 선분으로 구성 가능
typedef struct {
    Point vertices[3]; // 삼각형의 세 꼭짓점
    Line edges[3];     // 삼각형의 세 변 (옵션, 필요시 정의)
} GameObject_Triangle;
#endif