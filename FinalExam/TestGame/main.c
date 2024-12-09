#include "ElfMath.h"
#include "Elf2D.h"
#include "ElfTime.h"
#include "ElfObject.h"
#include "conio.h"

#define WIDTH 40    // 가로 픽셀 개수
#define HEIGHT 24   // 세로 픽셀 개수

char screenBuffer[(WIDTH + 1) * 2 * HEIGHT];
char screenWidth = WIDTH;
char screenHeight = HEIGHT;


void Initialize(GameObject_Line* obj, int objNum)
{
    //객체 초기화(관절 3개)
    int i = 0;
    for (i = 0; i < 3; i++)
    {
        obj[i].Position.x = 0;
        obj[i].Position.y = 0;

        obj[i].Rotation = 0;


        //(0,0) ~ (3,0) 을 잇는 직선
        obj[i].Line[0].x = 0;
        obj[i].Line[0].y = 0;

        obj[i].Line[1].x = 3;
        obj[i].Line[1].y = 0;
    }

    obj[1].Rotation = 30;
    obj[2].Rotation = 30;

    //화면 출력 심볼 입력
    obj[0].Symbol = "11";
    obj[1].Symbol = "22";
    obj[2].Symbol = "33";

    //크기 입력
    obj[0].Scale.x = 1;
    obj[0].Scale.y = 1;
    obj[1].Scale.x = 2;
    obj[1].Scale.y = 2;
    obj[2].Scale.x = 2;
    obj[2].Scale.y = 2;
}

// 0: 입력없음
// 99 : 게임 종료
int Input()
{
    // ESC 키를 눌렀는지 확인
    if (_kbhit()) {
        char ch = _getch();
        if (ch == 27) {  // ESC 키가 눌리면 게임 종료
            return 99;
        }
    }

    return 0;
}

void Update(GameObject_Line* obj, int objNum, int e)
{

}

void Render(GameObject_Line* obj, int objNum, char* Buf, int width, int height)
{
    Vector3 lineA, lineB;
    Matrix3x3 world = identity_matrix();    //단위행렬로 초기화

    lineB.x = 0;
    lineB.y = 0;

    int i = 0;

    obj[i].Position.x = lineB.x;
    obj[i].Position.y = lineB.y;

    lineA.x = obj[i].Line[0].x;
    lineA.y = obj[i].Line[0].y;
    lineA.z = 1;
    lineB.x = obj[i].Line[1].x;
    lineB.y = obj[i].Line[1].y;
    lineB.z = 1;

    Matrix3x3 scale = scale_matrix(obj[i].Scale.x, obj[i].Scale.y); // 크기조정 행렬 적용
    Matrix3x3 rotation = rotation_matrix(obj[i].Rotation);// 회전 행렬 생성
    Matrix3x3 translation = translation_matrix(obj[i].Position.x, obj[i].Position.y); // 이동 행렬 적용

    world = multiply_matrices(scale, world);
    world = multiply_matrices(rotation, world);
    world = multiply_matrices(translation, world);
    lineA = multiply_matrix_vector(world, lineA);  // 점과 회전 행렬 곱셈 (Matrix3x3 * Vector3)
    lineB = multiply_matrix_vector(world, lineB);  // 점과 회전 행렬 곱셈 (Matrix3x3 * Vector3)

    Elf2DDrawLine((int)lineA.x, (int)lineA.y, (int)lineB.x, (int)lineB.y, Buf, width, height);
}

void Elf2DDrawline2(Point start, Point end, char draw_char) {
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    int steps = (fabs(dx) > fabs(dy)) ? (int)fabs(dx) : (int)fabs(dy);

    float x_inc = dx / steps;
    float y_inc = dy / steps;

    float x = start.x;
    float y = start.y;

    for (int i = 0; i <= steps; i++) {
        int screenWidth_x = (int)x;
        int screenHeight_y = (int)y;

        if (screenWidth_x >= 0 && screenWidth_x < WIDTH && screenHeight_y >= 0 && screenHeight_y < HEIGHT) {
            screenBuffer[screenHeight_y * screenWidth + screenWidth_x] = draw_char;
        }

        x += x_inc;
        y += y_inc;
    }
}

// 삼각형 그리기 함수
void DrawTriangle(Point center, float distance, char draw_char) {
    // 삼각형 꼭짓점 계산
    Point top = { center.x, center.y - distance };                   // 위쪽 꼭짓점
    Point left = { center.x - distance / 2, center.y + distance / 2 }; // 왼쪽 꼭짓점
    Point right = { center.x + distance / 2, center.y + distance / 2 }; // 오른쪽 꼭짓점

    // 삼각형의 변을 그리기
    Elf2DDrawline2(top, left, draw_char);   // 왼쪽 변
    Elf2DDrawline2(left, right, draw_char); // 아래 변
    Elf2DDrawline2(right, top, draw_char);  // 오른쪽 변
}

// 게임 루프
int main() {
    int fps = 30;
    double frameTime = 1000.0 / fps;

    // 전역 변수로 스크린 버퍼 선언
    char screenBuffer[(WIDTH + 1) * HEIGHT];
    int screenWidth = WIDTH;
    int screenHeight = HEIGHT;

    GameObject_Line LineObj[3];

    // 게임 초기화
    Initialize(LineObj, 3);

    Elf2DInitScreen();
    Elf2DClearScreen(screenBuffer, screenWidth, screenHeight);
    Elf2DDrawBuffer(screenBuffer);

    ElfTimeInitialize();


    int isGameRunning = 1;
    while (isGameRunning) {

        int gameEvent = Input();

        //입력 계산
        if (gameEvent == 99)
        {
            isGameRunning = 0;
        }

        // 게임 업데이트
        Update(LineObj, 3, gameEvent);

        // 스크린버퍼에 게임 렌더링
        Render(LineObj, 3, screenBuffer, screenWidth, screenHeight);

        // 화면 그리기
        Elf2DDrawBuffer(screenBuffer);

        ElfTimeCalculateDeltaTime();
        Elf2DSleep((int)(frameTime - ElfTimeGetDeltaTime()));
    }

    return 0;
}
