#include <stdio.h>
#include <math.h>
#include <windows.h>

#define WIDTH 21
#define HEIGHT 11
#define PI 3.14159265358979323846

typedef struct {
    double x;
    double y;
} Vector2D;

typedef struct {
    double m[2][2];
} Matrix2D;

Matrix2D createRotationMatrix(double angle) {
    Matrix2D rot;
    rot.m[0][0] = cos(angle);  rot.m[0][1] = -sin(angle);
    rot.m[1][0] = sin(angle);  rot.m[1][1] = cos(angle);
    return rot;
}

Vector2D matrixVectorMultiply(Matrix2D mat, Vector2D vec) {
    Vector2D result;
    result.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y;
    result.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y;
    return result;
}

void clearScreen() {
    system("cls");
}

void drawRectangle(Matrix2D rotationMatrix) {
    char rectangle[HEIGHT][WIDTH] = {0};
    
    // 배열 초기화
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            rectangle[i][j] = ' ';
        }
    }
    
    // 원본 사각형의 모든 점을 회전
    for (double y = -2; y <= 2; y += 0.5) {
        for (double x = -5; x <= 5; x += 0.5) {
            Vector2D point = {x, y};
            Vector2D rotated = matrixVectorMultiply(rotationMatrix, point);
            
            // 화면 좌표계로 변환
            int screen_x = (int)round(rotated.x + WIDTH/2);
            int screen_y = (int)round(rotated.y + HEIGHT/2);
            
            // 범위 체크 및 점찍기
            if (screen_x >= 0 && screen_x < WIDTH && 
                screen_y >= 0 && screen_y < HEIGHT) {
                rectangle[screen_y][screen_x] = '*';
            }
        }
    }
    
    // 가로 방향으로 빈 공간 채우기
    for (int i = 0; i < HEIGHT; i++) {
        int start = -1;
        for (int j = 0; j < WIDTH; j++) {
            if (rectangle[i][j] == '*') {
                if (start == -1) {
                    start = j;
                } else {
                    // 이전 '*'와 현재 '*' 사이의 공간을 채움
                    for (int k = start + 1; k < j; k++) {
                        rectangle[i][k] = '*';
                    }
                    start = j;
                }
            }
        }
    }
    
    // 세로 방향으로 빈 공간 채우기
    for (int j = 0; j < WIDTH; j++) {
        int start = -1;
        for (int i = 0; i < HEIGHT; i++) {
            if (rectangle[i][j] == '*') {
                if (start == -1) {
                    start = i;
                } else {
                    // 이전 '*'와 현재 '*' 사이의 공간을 채움
                    for (int k = start + 1; k < i; k++) {
                        rectangle[k][j] = '*';
                    }
                    start = i;
                }
            }
        }
    }
    
    // 화면에 출력
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", rectangle[i][j]);
        }
        printf("\n");
    }
}

int main() {
    double angle = 0;
    double rotation_speed = 2 * PI / 3;  // 120도/초
    
    while (1) {
        clearScreen();
        
        Matrix2D rotationMatrix = createRotationMatrix(angle);
        drawRectangle(rotationMatrix);
        
        angle += rotation_speed / 10;
        if (angle >= 2 * PI) {
            angle -= 2 * PI;
        }
        
        Sleep(100);
    }
    
    return 0;
}