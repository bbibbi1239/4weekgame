#include <stdio.h>
#include <math.h>
#include <windows.h>

#define WIDTH 40
#define HEIGHT 20

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    float x, y, w;
} Vec3;

typedef struct {
    float m[3][3]; // 3x3 행렬
} Matrix3x3;

// 3x3 행렬 초기화
void initIdentityMatrix(Matrix3x3* mat) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mat->m[i][j] = (i == j) ? 1 : 0;
        }
    }
}

// 이동 행렬 생성
Matrix3x3 createTranslationMatrix(float tx, float ty) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    mat.m[0][2] = tx; // X축 이동
    mat.m[1][2] = ty; // Y축 이동
    return mat;
}

// 회전 행렬 생성
Matrix3x3 createRotationMatrix(float angle) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    float radian = angle * M_PI / 180;
    mat.m[0][0] = cos(radian); // cos(θ)
    mat.m[0][1] = -sin(radian); // -sin(θ)
    mat.m[1][0] = sin(radian); // sin(θ)
    mat.m[1][1] = cos(radian); // cos(θ)
    return mat;
}

// 점 변환
Vec3 transformPoint(Vec3 vec, Matrix3x3 mat) {
    Vec3 result;
    result.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.w;
    result.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.w;
    result.w = vec.w; // w는 그대로 유지
    return result;
}

// 콘솔 화면 지우기
void clearScreen() {
    system("cls");
}

// 화면 그리기
void draw(int angle) {
    clearScreen();

    // P의 위치 (고정)
    int pX = WIDTH / 2;
    int pY = HEIGHT / 2;

    // O의 초기 위치 (P와의 거리)
    float radiusO = 5.0f; // O의 회전 반경
    Vec3 o = { radiusO, 0.0f, 1.0f }; // O의 상대적 위치

    // Q의 초기 위치 (O와의 거리)
    float radiusQ = 2.0f; // Q의 회전 반경
    Vec3 q = { radiusQ, 0.0f, 1.0f }; // Q의 상대적 위치

    // O의 회전
    float oAngle = angle; // O의 회전 각도
    Vec3 oTransformed;
    oTransformed.x = pX + radiusO * cos(oAngle * M_PI / 180);
    oTransformed.y = pY + radiusO * sin(oAngle * M_PI / 180);
    oTransformed.w = 1.0f; // w는 그대로 유지

    // Q의 회전 (O의 위치를 중심으로)
    float qAngle = angle * 2; // Q는 O를 기준으로 더 빠르게 회전
    Vec3 qTransformed;
    qTransformed.x = oTransformed.x + radiusQ * cos(qAngle * M_PI / 180);
    qTransformed.y = oTransformed.y + radiusQ * sin(qAngle * M_PI / 180);
    qTransformed.w = 1.0f; // w는 그대로 유지

    // 화면 그리기
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == pX && y == pY) {
                printf("P"); // P 출력
            }
            else if ((int)(oTransformed.x) == x && (int)(oTransformed.y) == y) {
                printf("O"); // O 출력
            }
            else if ((int)(qTransformed.x) == x && (int)(qTransformed.y) == y) {
                printf("Q"); // Q 출력
            }
            else {
                printf(" "); // 빈 공간 출력
            }
        }
        printf("\n");
    }
}

int main() {
    for (int angle = 0; angle < 360; angle += 5) {
        draw(angle); // 회전 각도에 따라 그리기
        Sleep(100); // 0.1초 대기
    }
    return 0;
}
