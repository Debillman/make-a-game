#include "ElfMath.h"
#include "Elf2D.h"
#include "ElfTime.h"
#include "ElfObject.h"
#include <math.h>
#include <conio.h>

#define WIDTH 80
#define HEIGHT 24

float currentRotation = 0.0f;  // ���� ȸ�� ����

// �ﰢ�� �ʱ�ȭ
void Initialize(GameObject_Line* obj, int objNum)
{
    obj[0].Line[0].x = -10; // ù ��° ������
    obj[0].Line[0].y = -5;
    obj[0].Line[1].x = 10; // �� ��° ������
    obj[0].Line[1].y = -5;

    obj[1].Line[0].x = 10; // �� ��° ������
    obj[1].Line[0].y = -5;
    obj[1].Line[1].x = 0; // �� ��° ������
    obj[1].Line[1].y = 10;

    obj[2].Line[0].x = 0; // �� ��° ������
    obj[2].Line[0].y = 10;
    obj[2].Line[1].x = -10; // ù ��° ���������� ȸ��
    obj[2].Line[1].y = -5;

    for (int i = 0; i < objNum; i++) {
        obj[i].Position.x = 0; // �ﰢ�� �߽�
        obj[i].Position.y = 0;
        obj[i].Rotation = 0;
        obj[i].Scale.x = 1.0f; // �⺻ ������
        obj[i].Scale.y = 1.0f;
        obj[i].Symbol = "11";
    }
}

// Ű �Է� ó��
int Input()
{
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 27) {
            return 99; // ESC Ű�� ����
        }
        else if (ch == 'a') {
            return -1; // ���� ȸ��
        }
        else if (ch == 'd') {
            return 1; // ������ ȸ��
        }
        else if (ch == 'w') {
            return 'w'; // 'w' Ű�� ����
        }
        else if (ch == 's') {
            return 's'; // 's' Ű�� ����
        }
    }
    return 0; // �Է� ����
}

// �ﰢ���� '++' ������ �������� �����ϰų� ������Ű�� ���� �Լ�
void MoveObject(GameObject_Line* obj, char direction)
{
    float moveSpeed = 0.1f;  // ���� ���� �ӵ�

    // ���� ȸ�� �������� '++' ������ ���Ѵ�
    float radians = currentRotation * (3.14159265359f / 180.0f);  // ������ �������� ��ȯ
    Vector2 forwardDirection = { cosf(radians), sinf(radians) };  // ȸ�� ���⿡ �ش��ϴ� ���� ����

    // �ﰢ���� �������� �̵� (w: forward, s: backward)
    for (int i = 0; i < 3; i++) {
        if (direction == 'w') {
            // ����
            obj[i].Position.x += forwardDirection.x * moveSpeed;
            obj[i].Position.y += forwardDirection.y * moveSpeed;
        }
        else if (direction == 's') {
            // ����
            obj[i].Position.x -= forwardDirection.x * moveSpeed;
            obj[i].Position.y -= forwardDirection.y * moveSpeed;
        }
    }
}

// ���� ������Ʈ
void Update(GameObject_Line* obj, int objNum, int e)
{
    if (e == -1) {
        currentRotation -= 1.0f; // ��ȸ��
        if (currentRotation < 0.0f) {
            currentRotation += 360.0f;
        }
    }
    else if (e == 1) {
        currentRotation += 1.0f; // ��ȸ��
        if (currentRotation >= 360.0f) {
            currentRotation -= 360.0f;
        }
    }
}

// ��� �������� "++" ��Ʈ �׸���
void DrawDotAtPoint(Vector3 point, char* Buf, int width, int height)
{
    int x = (int)round(point.x);
    int y = (int)round(point.y);

    // ȭ�� �ȿ� ���� ���� �׸���
    if (x >= 0 && x < width - 1 && y >= 0 && y < height - 1) {
        Buf[y * (width + 1) + x] = '+';
        Buf[y * (width + 1) + x + 1] = '+';
    }
}

// ȭ�� ������
void Render(GameObject_Line* obj, int objNum, char* Buf, int width, int height)
{
    Elf2DClearScreen(Buf, width, height); // ���� �ʱ�ȭ

    Matrix3x3 world;
    Vector3 lineA, lineB;
    Vector3 topVertex = { 0, -INFINITY, 1 }; // ��� �������� �ʱ�ȭ, y���� �ſ� ���� ������ ����

    for (int i = 0; i < objNum; i++) {
        world = identity_matrix();

        lineA.x = obj[i].Line[0].x + obj[i].Position.x; // ��ġ�� ����� x ��ǥ
        lineA.y = obj[i].Line[0].y + obj[i].Position.y; // ��ġ�� ����� y ��ǥ
        lineA.z = 1;
        lineB.x = obj[i].Line[1].x + obj[i].Position.x;
        lineB.y = obj[i].Line[1].y + obj[i].Position.y;
        lineB.z = 1;

        // ��� ����
        Matrix3x3 scale = scale_matrix(obj[i].Scale.x, obj[i].Scale.y);
        Matrix3x3 rotation = rotation_matrix(currentRotation);
        Matrix3x3 translation = translation_matrix(width / 2, height / 2); // ȭ�� �߽ɿ� ��ġ

        world = multiply_matrices(scale, world);
        world = multiply_matrices(rotation, world);
        world = multiply_matrices(translation, world);

        // ��ȯ�� ��ǥ ���
        lineA = multiply_matrix_vector(world, lineA);
        lineB = multiply_matrix_vector(world, lineB);

        // ��� ������ ã�� (y���� ���� ū ��)
        if (lineA.y > topVertex.y) {
            topVertex = lineA;
        }
        if (lineB.y > topVertex.y) {
            topVertex = lineB;
        }

        // �ﰢ�� �� �׸���
        Elf2DDrawLine2(round(lineA.x), round(lineA.y), round(lineB.x), round(lineB.y), Buf, width, height);
    }

    // ��� �������� "++" ��Ʈ ǥ��
    DrawDotAtPoint(topVertex, Buf, width, height);
}


int main()
{
    int fps = 30;
    double frameTime = 1000.0 / fps;

    char screenBuffer[(WIDTH + 1) * HEIGHT];
    int screenWidth = WIDTH;
    int screenHeight = HEIGHT;

    GameObject_Line LineObj[3]; // �ﰢ�� ����

    Initialize(LineObj, 3); // �ﰢ�� �ʱ�ȭ

    Elf2DInitScreen();
    ElfTimeInitialize();

    int isGameRunning = 1;
    while (isGameRunning) {
        int gameEvent = Input();
        if (gameEvent == 99) { // ESC �Է� �� ����
            isGameRunning = 0;
        }
        else if (gameEvent == 'w' || gameEvent == 's') {
            // 'w' �Ǵ� 's' Ű �Է� �� �̵� ó��
            MoveObject(LineObj, (char)gameEvent);
        }

        Update(LineObj, 3, gameEvent); // ȸ���� ������Ʈ
        Render(LineObj, 3, screenBuffer, screenWidth, screenHeight); // ������
        Elf2DDrawBuffer(screenBuffer); // ���� ���

        // ������ ó��
        ElfTimeCalculateDeltaTime();
        Elf2DSleep((int)round(frameTime - ElfTimeGetDeltaTime()));
    }

    return 0;
}
