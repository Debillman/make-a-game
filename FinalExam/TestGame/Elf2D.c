#include <stdio.h>
#include <Windows.h>
#include "Elf2D.h"

void Elf2DInitScreen()
{
    // 커서 숨기기
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = FALSE;  // 커서를 숨김
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void Elf2DClearScreen(char* Buffer, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Buffer[i * (width + 1) + j] = ' ';  // 화면 초기화
        }
        Buffer[i * (width + 1) + width] = '\n';  // 개행 문자
    }
    Buffer[(width + 1) * height - 1] = '\0';  // 개행 문자
}

void Elf2DDrawBuffer(char* Buffer)
{
    // 0,0으로 이동
    COORD coord = { 0, 0 };  // 좌표 (0, 0)
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    // 스크린 버퍼 출력
    printf("%s", Buffer);  // 화면 출력
}

void Elf2DDrawLine(int x1, int y1, int x2, int y2, char* Buffer, int width, int height)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < height)
        {
            Buffer[y1 * (width + 1) + x1] = '*';  // 화면에 점을 찍음
        }

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = err * 2;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}


#include <math.h>

void Elf2DDrawLine2(float x1, float y1, float x2, float y2, char* Buffer, int width, int height)
{
    float dx = fabs(x2 - x1); // 절댓값
    float dy = fabs(y2 - y1); // 절댓값
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    float err = dx - dy;

    // 상단 꼭짓점 정의
    int topX = width / 2;  // 화면 중앙 기준
    int topY = 0;          // 삼각형의 상단 y 좌표

    while (1) {
        int ix1 = (int)roundf(x1); // float 좌표를 정수로 변환 (라운드)
        int iy1 = (int)roundf(y1); // float 좌표를 정수로 변환 (라운드)

        if (ix1 >= 0 && ix1 < width && iy1 >= 0 && iy1 < height)
        {
            if (ix1 == topX && iy1 == topY) {
                // 상단 꼭짓점에서 ++ 출력
                Buffer[iy1 * (width + 1) + ix1] = '+';
                if (ix1 + 1 < width) { // 오른쪽에 추가 '+' 출력
                    Buffer[iy1 * (width + 1) + ix1 + 1] = '+';
                }
            }
            else {
                // 나머지는 @ 출력
                Buffer[iy1 * (width + 1) + ix1] = '@';
                Buffer[iy1 * (width + 1) + ix1 + 1] = '@';
            }
        }

        if (fabs(x1 - x2) < 0.01 && fabs(y1 - y2) < 0.01) // 부동소수점 비교 (오차 범위)
            break;

        float e2 = err * 2;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void Elf2DSleep(int ms)
{
    Sleep(ms);
}
