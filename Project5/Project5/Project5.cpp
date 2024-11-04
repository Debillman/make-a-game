#include <stdio.h>
#include <math.h>
#include <time.h>

#define WIDTH 11
#define HEIGHT 5
#define FPS 30
#define PI 3.14159265
#define SCREEN_SIZE 20

// 회전 각도를 라디안으로 변환
double toRadians(double degrees) {
    return degrees * (PI / 180);
}

// (x, y) 좌표를 회전시키는 함수
void rotatePoint(int cx, int cy, int* x, int* y, double angle) {
    int tempX = *x - cx;
    int tempY = *y - cy;

    *x = cx + (int)(tempX * cos(angle) - tempY * sin(angle) + 0.5);
    *y = cy + (int)(tempX * sin(angle) + tempY * cos(angle) + 0.5);
}

// 두 점 사이의 직선을 촘촘히 그리는 함수
void drawDenseLine(char screen[SCREEN_SIZE][SCREEN_SIZE], int x0, int y0, int x1, int y1, int offsetX, int offsetY) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        if (x0 + offsetX >= 0 && x0 + offsetX < SCREEN_SIZE && y0 + offsetY >= 0 && y0 + offsetY < SCREEN_SIZE) {
            screen[y0 + offsetY][x0 + offsetX] = '*';
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// 사각형 내부를 채우며 그리기 함수
void drawFilledRectangle(int angle) {
    char screen[SCREEN_SIZE][SCREEN_SIZE];
    for (int i = 0; i < SCREEN_SIZE; i++) {
        for (int j = 0; j < SCREEN_SIZE; j++) {
            screen[i][j] = ' ';  // 빈 공간 초기화
        }
    }

    // 사각형의 각 꼭짓점
    int x0 = -WIDTH / 2, y0 = -HEIGHT / 2;
    int x1 = WIDTH / 2, y1 = -HEIGHT / 2;
    int x2 = WIDTH / 2, y2 = HEIGHT / 2;
    int x3 = -WIDTH / 2, y3 = HEIGHT / 2;

    // 중심 좌표 (0, 0)
    int cx = 0, cy = 0;
    double radianAngle = toRadians(angle);

    // 각 꼭짓점을 회전
    rotatePoint(cx, cy, &x0, &y0, radianAngle);
    rotatePoint(cx, cy, &x1, &y1, radianAngle);
    rotatePoint(cx, cy, &x2, &y2, radianAngle);
    rotatePoint(cx, cy, &x3, &y3, radianAngle);

    int offsetX = SCREEN_SIZE / 2;
    int offsetY = SCREEN_SIZE / 2;

    // 꼭짓점 연결하여 사각형 외곽을 촘촘히 채우기
    drawDenseLine(screen, x0, y0, x1, y1, offsetX, offsetY);
    drawDenseLine(screen, x1, y1, x2, y2, offsetX, offsetY);
    drawDenseLine(screen, x2, y2, x3, y3, offsetX, offsetY);
    drawDenseLine(screen, x3, y3, x0, y0, offsetX, offsetY);

    // 사각형 내부를 더 세밀히 채우기 위해 한 줄씩 스캔
    for (int y = fmin(fmin(y0, y1), fmin(y2, y3)); y <= fmax(fmax(y0, y1), fmax(y2, y3)); y++) {
        int start = SCREEN_SIZE, end = -1;
        for (int x = 0; x < SCREEN_SIZE; x++) {
            if (screen[y + offsetY][x] == '*') {
                if (start == SCREEN_SIZE) start = x;
                end = x;
            }
        }
        if (start < end) {
            for (int x = start; x <= end; x++) {
                screen[y + offsetY][x] = '*';
            }
        }
    }

    // 화면 초기화 후 사각형 출력
    printf("\033[2J\033[H");
    printf("회전 각도: %d도\n", angle);

    for (int i = 0; i < SCREEN_SIZE; i++) {
        for (int j = 0; j < SCREEN_SIZE; j++) {
            printf("%c", screen[i][j]);
        }
        printf("\n");
    }
}

// 딜레이 함수 (밀리초 단위)
void delay(int milliseconds) {
    clock_t start_time = clock();
    while (clock() < start_time + milliseconds * CLOCKS_PER_SEC / 1000);
}

// 메인 함수
int main() {
    int angle = 0;

    while (1) {
        drawFilledRectangle(angle);
        angle = (angle + 120 / FPS) % 360;  // 초당 120도 회전하도록 속도 조정
        delay(1000 / FPS);  // 30 FPS에 맞춘 대기 시간
    }

    return 0;
}
