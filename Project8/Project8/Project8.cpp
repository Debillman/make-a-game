#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h> 
#include <windows.h>

#include "ElfMath.h"

#define WIDTH 40    // 가로 픽셀 개수
#define HEIGHT 24   // 세로 픽셀 개수
#define PIXEL_SIZE 2 // 한 픽셀당 문자 개수
#define PI 3.14159265358979323846

// 픽셀 구조체 정의
typedef struct {
    char c1; // 첫 번째 문자
    char c2; // 두 번째 문자
} Pixel;

// 화면 구조체 정의
typedef struct {
    Pixel buffer[HEIGHT][WIDTH]; // 2차원 배열로 구성된 화면 버퍼
} Screen;

// 벡터 구조체 정의
typedef struct {
    float x;
    float y;
} Vector2;

// 화면 초기화 함수
void ClearScreen(Screen* screen) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            screen->buffer[y][x].c1 = ' '; // 공백 문자로 초기화
            screen->buffer[y][x].c2 = ' ';
        }
    }
}

// 특정 위치에 픽셀을 그리는 함수
void Draw(Screen* screen, int x, int y, char c1, char c2) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return; // 범위를 벗어나면 무시
    }
    screen->buffer[y][x].c1 = c1;
    screen->buffer[y][x].c2 = c2;
}

// 문자열을 화면에 그리는 함수 (한글 지원)
void DrawText(Screen* screen, int x, int y, const wchar_t* text) {
    while (*text) {
        if (*text < 128) {
            Draw(screen, x++, y, (char)*text, ' ');
        } else {
            Draw(screen, x++, y, '?', ' ');
        }
        text++;
    }
}

// 콘솔 문자 집합을 UTF-8로 설정하는 함수
void SetConsoleUTF8() {
    // 콘솔 문자 인코딩을 UTF-8로 설정
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

// 삼각형 그리기 함수
void DrawFilledTriangle(Screen* screen, Vector2 center, float size, float angle, char c1, char c2) {
    Vector2 vertices[3];
    for (int i = 0; i < 3; i++) {
        float theta = angle + i * 2 * PI / 3;
        vertices[i].x = center.x + size * cos(theta);
        vertices[i].y = center.y + size * sin(theta);
    }
    for (int y = (int)(center.y - size); y <= (int)(center.y + size); y++) {
        for (int x = (int)(center.x - size); x <= (int)(center.x + size); x++) {
            float w1 = ((vertices[1].y - vertices[2].y) * (x - vertices[2].x) + (vertices[2].x - vertices[1].x) * (y - vertices[2].y)) /
                ((vertices[1].y - vertices[2].y) * (vertices[0].x - vertices[2].x) + (vertices[2].x - vertices[1].x) * (vertices[0].y - vertices[2].y));
            float w2 = ((vertices[2].y - vertices[0].y) * (x - vertices[2].x) + (vertices[0].x - vertices[2].x) * (y - vertices[2].y)) /
                ((vertices[1].y - vertices[2].y) * (vertices[0].x - vertices[2].x) + (vertices[2].x - vertices[1].x) * (vertices[0].y - vertices[2].y));
            float w3 = 1 - w1 - w2;
            if (w1 >= 0 && w2 >= 0 && w3 >= 0) {
                Draw(screen, x, y, c1, c2);
            }
        }
    }
}

// 정사각형 그리기 함수
void DrawFilledSquare(Screen* screen, Vector2 center, float size, float angle, char c1, char c2) {
    Vector2 vertices[4];
    for (int i = 0; i < 4; i++) {
        float theta = angle + i * PI / 2;
        vertices[i].x = center.x + size * cos(theta);
        vertices[i].y = center.y + size * sin(theta);
    }
    for (int y = (int)(center.y - size); y <= (int)(center.y + size); y++) {
        for (int x = (int)(center.x - size); x <= (int)(center.x + size); x++) {
            if (x >= vertices[0].x && x <= vertices[2].x && y >= vertices[0].y && y <= vertices[2].y) {
                Draw(screen, x, y, c1, c2);
            }
        }
    }
}

// 원 그리기 함수
void DrawFilledCircle(Screen* screen, Vector2 center, float radius, char c1, char c2) {
    for (int y = (int)(center.y - radius); y <= (int)(center.y + radius); y++) {
        for (int x = (int)(center.x - radius); x <= (int)(center.x + radius); x++) {
            float dx = x - center.x;
            float dy = y - center.y;
            if (dx * dx + dy * dy <= radius * radius) {
                Draw(screen, x, y, c1, c2);
            }
        }
    }
}

// 콘솔 화면에 픽셀 버퍼를 렌더링
void RenderScreen(Screen* screen) {
    COORD coord = { 0, 0 }; // 콘솔 출력 시작 위치
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            putchar(screen->buffer[y][x].c1); // 첫 번째 문자 출력
            putchar(screen->buffer[y][x].c2); // 두 번째 문자 출력
        }
        putchar('\n'); // 한 줄 출력 후 개행
    }
}

int main() {
    // 콘솔 문자 집합을 UTF-8로 설정
    SetConsoleUTF8();

    Screen screen;
    int running = 1; // 프로그램 실행 여부를 나타내는 플래그
    float angle1 = 0, angle2 = 0, angle3 = 0; // 각 객체의 자전 각도
    float orbitAngle2 = 0, orbitAngle3 = 0; // 각 객체의 공전 각도

    // 화면 초기화
    ClearScreen(&screen);

    // 중앙에 텍스트 그리기 (한글 텍스트)
    DrawText(&screen, WIDTH / 2 - 8, HEIGHT / 2, L"20211400 김승우");

    // 화면 렌더링
    RenderScreen(&screen);

    // 스페이스바 입력 대기
    while (1) {
        if (_kbhit()) { // 키보드 입력 감지
            char key = _getch(); // 키 입력 받기
            if (key == ' ') { // 스페이스바
                break; // 루프 종료
            }
        }
    }

    // 화면 초기화
    ClearScreen(&screen);

    Vector2 center1 = { 20, 12 };
    Vector2 center2 = { center1.x + 12, center1.y };
    Vector2 center3 = { center2.x + 4, center2.y };

    // 객체들이 도는 화면
    while (running) {
        // 화면 초기화
        ClearScreen(&screen);

        // 객체 1 (삼각형) 그리기
        DrawFilledTriangle(&screen, center1, 5, angle1, 'S', 'S');

        // 객체 2 (정사각형) 그리기
        Vector2 orbitCenter2 = { center1.x + 12 * cos(orbitAngle2), center1.y + 12 * sin(orbitAngle2) };
        DrawFilledSquare(&screen, orbitCenter2, 3, angle2, 'E', 'E');

        // 객체 3 (원) 그리기
        Vector2 orbitCenter3 = { orbitCenter2.x + 4 * cos(orbitAngle3), orbitCenter2.y + 4 * sin(orbitAngle3) };
        DrawFilledCircle(&screen, orbitCenter3, 1.5, 'M', 'M');

        // 화면 렌더링
        RenderScreen(&screen);

        // 키 입력 처리
        if (_kbhit()) { // 키보드 입력 감지
            char key = _getch(); // 키 입력 받기
            if (key == 27) { // ESC 키 (ASCII 27)
                running = 0; // 루프 종료
                break;
            }
        }

        // 각도 업데이트
        angle1 -= 2 * PI / (3 * 10); // 3초당 1바퀴 (반시계방향)
        angle2 += 2 * PI / (2 * 10); // 2초당 1바퀴 (시계방향)
        angle3 -= 2 * PI / 10; // 1초당 1바퀴 (반시계방향)
        orbitAngle2 += 2 * PI / (3 * 10); // 3초당 1바퀴 (시계방향)
        orbitAngle3 -= 2 * PI / (2 * 10); // 2초당 1바퀴 (반시계방향)

        // 루프 속도 조절
        Sleep(100); // 100 밀리초 대기
    }

    return 0;
}