#ifndef ELFOBJECT_H  // ���ǵǾ� ���� ������
#define ELFOBJECT_H  // �����ϰ� �Ʒ� �ڵ带 ������

#include "ElfMath.h"


//���� ������Ʈ
typedef struct
{
    //Transform
    Vector2 Position;
    float Rotation;
    Vector2 Scale;

    //Geometry
    Vector2 Line[2];

    //�ȼ�ä�� ����
    char* Symbol;

}GameObject_Line;

typedef struct {
    float x; // x ��ǥ
    float y; // y ��ǥ
} Point;

// ����(Line)�� ����: �� ������ ����
typedef struct {
    Point start; // ������ ������
    Point end;   // ������ ����
} Line;

// �ﰢ��(Triangle)�� ����: �� ���� �� �������� ���� ����
typedef struct {
    Point vertices[3]; // �ﰢ���� �� ������
    Line edges[3];     // �ﰢ���� �� �� (�ɼ�, �ʿ�� ����)
} GameObject_Triangle;
#endif