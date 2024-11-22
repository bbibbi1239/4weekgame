#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
#include "ElfMath.h"

#define WIDTH 40
#define HEIGHT 24
#define SUN_CENTER_X 20
#define SUN_CENTER_Y 12

char screenBuffer[HEIGHT * WIDTH * 2];

void clear_screen() {
    for (int i = 0; i < HEIGHT * WIDTH * 2; i++) {
        screenBuffer[i] = ' ';
    }
}

void draw_point(int x, int y, char c1, char c2) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        int index = (y * WIDTH + x) * 2;
        screenBuffer[index] = c1;
        screenBuffer[index + 1] = c2;
    }
}

// 삼각형 내부의 한 점이 삼각형 안에 있는지 확인하는 함수
int is_point_in_triangle(float px, float py, float x1, float y1, float x2, float y2, float x3, float y3) {
    float alpha = ((y2 - y3) * (px - x3) + (x3 - x2) * (py - y3)) /
        ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
    float beta = ((y3 - y1) * (px - x3) + (x1 - x3) * (py - y3)) /
        ((y2 - y3) * (x1 - x3) + (x3 - x2) * (y1 - y3));
    float gamma = 1.0f - alpha - beta;

    return (alpha >= 0 && beta >= 0 && gamma >= 0);
}

// 채워진 삼각형 그리기 (태양)
void draw_filled_sun(Vector3 center, float angle) {
    Vector3 vertices[3] = {
        {0, -5, 1},      // 상단
        {-4.33f, 2.5f, 1}, // 좌하단
        {4.33f, 2.5f, 1}   // 우하단
    };

    Matrix3x3 rot = rotation_matrix(angle);
    Matrix3x3 trans = translation_matrix(center.x, center.y);
    Matrix3x3 transform = multiply_matrices(trans, rot);

    // 변환된 꼭지점 계산
    Vector3 transformed[3];
    float minX = WIDTH, minY = HEIGHT, maxX = 0, maxY = 0;

    for (int i = 0; i < 3; i++) {
        transformed[i] = multiply_matrix_vector(transform, vertices[i]);
        minX = fminf(minX, transformed[i].x);
        minY = fminf(minY, transformed[i].y);
        maxX = fmaxf(maxX, transformed[i].x);
        maxY = fmaxf(maxY, transformed[i].y);
    }

    // 바운딩 박스 내의 모든 픽셀을 검사하여 삼각형 내부 채우기
    for (int y = (int)minY; y <= (int)maxY; y++) {
        for (int x = (int)minX; x <= (int)maxX; x++) {
            if (is_point_in_triangle(x, y,
                transformed[0].x, transformed[0].y,
                transformed[1].x, transformed[1].y,
                transformed[2].x, transformed[2].y)) {
                draw_point(x, y, 'S', 'S');
            }
        }
    }
}
//채워진 지구 그리기
void draw_filled_earth(Vector3 center, float angle) {
    Vector3 vertices[4] = {
        {-1, -1, 1},  // 좌하단 (-3 -> -1.5)
        {-1, 1, 1},   // 좌상단 (-3 -> -1.5, 3 -> 1.5)
        {1, 1, 1},    // 우상단 (3 -> 1.5)
        {1, -1, 1}    // 우하단 (3 -> 1.5, -3 -> -1.5)
    };

    Matrix3x3 rot = rotation_matrix(angle);
    Matrix3x3 trans = translation_matrix(center.x, center.y);
    Matrix3x3 transform = multiply_matrices(trans, rot);

    // 변환된 꼭지점 계산
    Vector3 transformed[4];
    float minX = WIDTH, minY = HEIGHT, maxX = 0, maxY = 0;
    for (int i = 0; i < 4; i++) {
        transformed[i] = multiply_matrix_vector(transform, vertices[i]);
        minX = fminf(minX, transformed[i].x);
        minY = fminf(minY, transformed[i].y);
        maxX = fmaxf(maxX, transformed[i].x);
        maxY = fmaxf(maxY, transformed[i].y);
    }

    // 바운딩 박스 내의 모든 픽셀을 검사하여 사각형 내부 채우기
    for (int y = (int)minY; y <= (int)maxY; y++) {
        for (int x = (int)minX; x <= (int)maxX; x++) {
            draw_point(x, y, 'E', 'E');
        }
    }
}

// 채워진 원 그리기 (달)
void draw_filled_moon(Vector3 center) {
    int radius = 1;  // 지름 3의 반지름

    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                int px = (int)(center.x + x + 0.5f);
                int py = (int)(center.y + y + 0.5f);
                draw_point(px, py, 'M', 'M');
            }
        }
    }
}

void display_screen() {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH * 2; x++) {
            putchar(screenBuffer[y * WIDTH * 2 + x]);
        }
        putchar('\n');
    }
}

int main() {
    Vector3 sun_pos = { SUN_CENTER_X, SUN_CENTER_Y, 1 };
    Vector3 earth_offset = { 12, 0, 1 };
    Vector3 moon_offset = { 4, 0, 1 };

    clock_t start_time = clock();
    float elapsed = 0;
    int program_state = 0;  // 0: 초기 화면, 1: 일렬 배치, 2: 공전/자전

    while (1) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 27) {  // ESC
                exit(0);
            }
            else if (key == 32) {  // 스페이스바
                if (program_state == 2) {
                    program_state = 1;  // 2에서 1로
                }
                else {
                    program_state = (program_state + 1) % 3;  // 0 -> 1, 1 -> 2
                }  // 0 -> 1 -> 2 -> 0 순환

                if (program_state == 2) {
                    // 공전/자전 시작할 때 시작 시간 초기화
                    start_time = clock();
                }
            }
        }

        clear_screen();
        system("cls");

        if (program_state == 0) {
            // 초기 화면: 학번과 이름 표시
            const char* info = "20231363 DAYE LEE";
            int text_x = (WIDTH - strlen(info)) / 2;
            int text_y = HEIGHT / 2;

            for (int i = 0; i < strlen(info); i++) {
                draw_point(text_x + i, text_y, info[i], ' ');
            }
        }
        else if (program_state == 1) {
            // 일렬 배치 상태
            Vector3 sun_pos_aligned = { SUN_CENTER_X, SUN_CENTER_Y, 1 };
            Vector3 earth_pos = { SUN_CENTER_X + 12, SUN_CENTER_Y, 1 };  // 태양 오른쪽 12칸
            Vector3 moon_pos = { earth_pos.x + 4, earth_pos.y, 1 };      // 지구 오른쪽 4칸

            draw_filled_sun(sun_pos_aligned, 0);
            draw_filled_earth(earth_pos, 0);
            draw_filled_moon(moon_pos);
        }
        else if (program_state == 2) {
            // 공전/자전 시작
            elapsed = (float)(clock() - start_time) / CLOCKS_PER_SEC;

            // 태양 자전
            float sun_rotation = (elapsed / 3.0f) * 360.0f;
            draw_filled_sun(sun_pos, sun_rotation);

            // 지구 공전 및 자전
            float earth_orbit = -(elapsed / 3.0f) * 360.0f;
            float earth_rotation = -(elapsed / 2.0f) * 360.0f;

            // 지구 위치 계산
            Matrix3x3 earth_orbit_transform = multiply_matrices(
                translation_matrix(SUN_CENTER_X, SUN_CENTER_Y),
                rotation_matrix(earth_orbit)
            );
            Vector3 earth_pos = multiply_matrix_vector(earth_orbit_transform, earth_offset);
            draw_filled_earth(earth_pos, earth_rotation);

            // 달의 공전
            float moon_orbit = (elapsed / 2.0f) * 360.0f;

            // 달 위치 계산
            Matrix3x3 moon_orbit_transform = multiply_matrices(
                translation_matrix(earth_pos.x, earth_pos.y),
                rotation_matrix(moon_orbit)
            );
            Vector3 moon_pos = multiply_matrix_vector(moon_orbit_transform, moon_offset);
            draw_filled_moon(moon_pos);
        }

        display_screen();
        Sleep(16);
    }

    return 0;
}