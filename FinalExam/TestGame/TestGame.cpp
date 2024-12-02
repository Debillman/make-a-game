#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  // _kbhit(), _getch() 함수 사용을 위한 헤더
#include <windows.h>
// 상수 정의
#define WIDTH 40
#define HEIGHT 24
#define FPS 30
#define FRAME_TIME (1.0f / FPS)

#include "ElfMath.h"
#include "Elf2D.h"

// 화면 버퍼 초기화 (각 픽셀을 2개의 문자로 표현)
char screen[HEIGHT][WIDTH * 2 + 1];

// 화면 지우기 함수
void clearScreen() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            screen[y][x * 2] = ' ';      // 첫 번째 문자
            screen[y][x * 2 + 1] = ' ';  // 두 번째 문자
        }
        screen[y][WIDTH * 2] = '\0';     // 문자열 끝
    }
}

// 선 그리기 함수
void drawLine(float x0, float y0, float x1, float y1, const char* symbol) {
    float dx = fabs(x1 - x0);
    float dy = fabs(y1 - y0);
    float sx = (x0 < x1) ? 1.0f : -1.0f;
    float sy = (y0 < y1) ? 1.0f : -1.0f;
    float err = dx - dy;

    while (1) {
        screen[(int)roundf(y0)][(int)roundf(x0 * 2)] = symbol[0];      // 첫 번째 문자
        screen[(int)roundf(y0)][(int)roundf(x0 * 2 + 1)] = symbol[1];  // 두 번째 문자
        if (fabs(x0 - x1) < 0.01f && fabs(y0 - y1) < 0.01f) break;
        float e2 = 2.0f * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

// 화면 출력 함수
void displayScreen() {
    printf("\033[H"); // 커서 이동
    for (int y = 0; y < HEIGHT; y++) {
        printf("%s\n", screen[y]);
    }
}

// 게임 루프
void GameLoop() {
    while (1) {
        clock_t start_time = clock();

        clearScreen();

        // (0,0)에서 (40,24)까지 선을 그리기
        drawLine(0.0f, 0.0f, WIDTH - 1.0f, HEIGHT - 1.0f, "@@");

        displayScreen();

        // 프레임 속도 조정
        while ((float)(clock() - start_time) / CLOCKS_PER_SEC < FRAME_TIME);
    }
}

int main() {
    printf("\033[2J"); // 화면 지우기
    GameLoop();
    return 0;
}
