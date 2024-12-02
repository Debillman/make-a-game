#include <stdio.h>
#include <time.h>
#include <math.h>

// 상수 정의
#define WIDTH 40
#define HEIGHT 24
#define FPS 30
#define FRAME_TIME (1.0f / FPS)

// 화면 버퍼 초기화 (char 크기를 2로 사용)
char screen[HEIGHT][WIDTH * 2 + 1]; // 각 픽셀은 2개의 문자로 표현됨

// 화면 지우기 함수
void clearScreen() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            screen[y][x * 2] = ' ';      // 첫 번째 문자 공간
            screen[y][x * 2 + 1] = ' ';  // 두 번째 문자 공간
        }
        screen[y][WIDTH * 2] = '\0';      // 문자열 끝
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
        
        // 삼각형을 그리기 위해 임시 선을 화면 중앙에 그리기
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
