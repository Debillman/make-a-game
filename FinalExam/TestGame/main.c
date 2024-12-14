#include "ElfMath.h"
#include "Elf2D.h"
#include "ElfTime.h"
#include "ElfObject.h"
#include <math.h>
#include <conio.h>

#define WIDTH 80
#define HEIGHT 24

float currentRotation = 0.0f;  // 현재 회전 각도

// 삼각형 초기화
void Initialize(GameObject_Line* obj, int objNum)
{
    obj[0].Line[0].x = -10; // 첫 번째 꼭짓점
    obj[0].Line[0].y = -5;
    obj[0].Line[1].x = 10; // 두 번째 꼭짓점
    obj[0].Line[1].y = -5;

    obj[1].Line[0].x = 10; // 두 번째 꼭짓점
    obj[1].Line[0].y = -5;
    obj[1].Line[1].x = 0; // 세 번째 꼭짓점
    obj[1].Line[1].y = 10;

    obj[2].Line[0].x = 0; // 세 번째 꼭짓점
    obj[2].Line[0].y = 10;
    obj[2].Line[1].x = -10; // 첫 번째 꼭짓점으로 회귀
    obj[2].Line[1].y = -5;

    for (int i = 0; i < objNum; i++) {
        obj[i].Position.x = 0; // 삼각형 중심
        obj[i].Position.y = 0;
        obj[i].Rotation = 0;
        obj[i].Scale.x = 1.0f; // 기본 스케일
        obj[i].Scale.y = 1.0f;
        obj[i].Symbol = "11";
    }
}

// 키 입력 처리
int Input()
{
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 27) {
            return 99; // ESC 키로 종료
        }
        else if (ch == 'a') {
            return -1; // 왼쪽 회전
        }
        else if (ch == 'd') {
            return 1; // 오른쪽 회전
        }
        else if (ch == 'w') {
            return 'w'; // 'w' 키는 전진
        }
        else if (ch == 's') {
            return 's'; // 's' 키는 후진
        }
    }
    return 0; // 입력 없음
}

// 삼각형을 '++' 꼭지점 방향으로 전진하거나 후진시키기 위한 함수
void MoveObject(GameObject_Line* obj, char direction)
{
    float moveSpeed = 0.1f;  // 전진 후진 속도

    // 현재 회전 각도에서 '++' 방향을 구한다
    float radians = currentRotation * (3.14159265359f / 180.0f);  // 각도를 라디안으로 변환
    Vector2 forwardDirection = { cosf(radians), sinf(radians) };  // 회전 방향에 해당하는 단위 벡터

    // 삼각형을 기준으로 이동 (w: forward, s: backward)
    for (int i = 0; i < 3; i++) {
        if (direction == 'w') {
            // 전진
            obj[i].Position.x += forwardDirection.x * moveSpeed;
            obj[i].Position.y += forwardDirection.y * moveSpeed;
        }
        else if (direction == 's') {
            // 후진
            obj[i].Position.x -= forwardDirection.x * moveSpeed;
            obj[i].Position.y -= forwardDirection.y * moveSpeed;
        }
    }
}

// 로직 업데이트
void Update(GameObject_Line* obj, int objNum, int e)
{
    if (e == -1) {
        currentRotation -= 1.0f; // 좌회전
        if (currentRotation < 0.0f) {
            currentRotation += 360.0f;
        }
    }
    else if (e == 1) {
        currentRotation += 1.0f; // 우회전
        if (currentRotation >= 360.0f) {
            currentRotation -= 360.0f;
        }
    }
}

// 상단 꼭짓점에 "++" 도트 그리기
void DrawDotAtPoint(Vector3 point, char* Buf, int width, int height)
{
    int x = (int)round(point.x);
    int y = (int)round(point.y);

    // 화면 안에 있을 때만 그리기
    if (x >= 0 && x < width - 1 && y >= 0 && y < height - 1) {
        Buf[y * (width + 1) + x] = '+';
        Buf[y * (width + 1) + x + 1] = '+';
    }
}

// 화면 렌더링
void Render(GameObject_Line* obj, int objNum, char* Buf, int width, int height)
{
    Elf2DClearScreen(Buf, width, height); // 버퍼 초기화

    Matrix3x3 world;
    Vector3 lineA, lineB;
    Vector3 topVertex = { 0, -INFINITY, 1 }; // 상단 꼭짓점을 초기화, y값은 매우 작은 값으로 설정

    for (int i = 0; i < objNum; i++) {
        world = identity_matrix();

        lineA.x = obj[i].Line[0].x + obj[i].Position.x; // 위치를 고려한 x 좌표
        lineA.y = obj[i].Line[0].y + obj[i].Position.y; // 위치를 고려한 y 좌표
        lineA.z = 1;
        lineB.x = obj[i].Line[1].x + obj[i].Position.x;
        lineB.y = obj[i].Line[1].y + obj[i].Position.y;
        lineB.z = 1;

        // 행렬 연산
        Matrix3x3 scale = scale_matrix(obj[i].Scale.x, obj[i].Scale.y);
        Matrix3x3 rotation = rotation_matrix(currentRotation);
        Matrix3x3 translation = translation_matrix(width / 2, height / 2); // 화면 중심에 배치

        world = multiply_matrices(scale, world);
        world = multiply_matrices(rotation, world);
        world = multiply_matrices(translation, world);

        // 변환된 좌표 계산
        lineA = multiply_matrix_vector(world, lineA);
        lineB = multiply_matrix_vector(world, lineB);

        // 상단 꼭짓점 찾기 (y값이 가장 큰 점)
        if (lineA.y > topVertex.y) {
            topVertex = lineA;
        }
        if (lineB.y > topVertex.y) {
            topVertex = lineB;
        }

        // 삼각형 선 그리기
        Elf2DDrawLine2(round(lineA.x), round(lineA.y), round(lineB.x), round(lineB.y), Buf, width, height);
    }

    // 상단 꼭짓점에 "++" 도트 표시
    DrawDotAtPoint(topVertex, Buf, width, height);
}


int main()
{
    int fps = 30;
    double frameTime = 1000.0 / fps;

    char screenBuffer[(WIDTH + 1) * HEIGHT];
    int screenWidth = WIDTH;
    int screenHeight = HEIGHT;

    GameObject_Line LineObj[3]; // 삼각형 구성

    Initialize(LineObj, 3); // 삼각형 초기화

    Elf2DInitScreen();
    ElfTimeInitialize();

    int isGameRunning = 1;
    while (isGameRunning) {
        int gameEvent = Input();
        if (gameEvent == 99) { // ESC 입력 시 종료
            isGameRunning = 0;
        }
        else if (gameEvent == 'w' || gameEvent == 's') {
            // 'w' 또는 's' 키 입력 시 이동 처리
            MoveObject(LineObj, (char)gameEvent);
        }

        Update(LineObj, 3, gameEvent); // 회전값 업데이트
        Render(LineObj, 3, screenBuffer, screenWidth, screenHeight); // 렌더링
        Elf2DDrawBuffer(screenBuffer); // 버퍼 출력

        // 프레임 처리
        ElfTimeCalculateDeltaTime();
        Elf2DSleep((int)round(frameTime - ElfTimeGetDeltaTime()));
    }

    return 0;
}
