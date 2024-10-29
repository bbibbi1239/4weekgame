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
    float m[3][3]; // 3x3 ���
} Matrix3x3;

// 3x3 ��� �ʱ�ȭ
void initIdentityMatrix(Matrix3x3* mat) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            mat->m[i][j] = (i == j) ? 1 : 0;
        }
    }
}

// �̵� ��� ����
Matrix3x3 createTranslationMatrix(float tx, float ty) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    mat.m[0][2] = tx; // X�� �̵�
    mat.m[1][2] = ty; // Y�� �̵�
    return mat;
}

// ȸ�� ��� ����
Matrix3x3 createRotationMatrix(float angle) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    float radian = angle * M_PI / 180;
    mat.m[0][0] = cos(radian); // cos(��)
    mat.m[0][1] = -sin(radian); // -sin(��)
    mat.m[1][0] = sin(radian); // sin(��)
    mat.m[1][1] = cos(radian); // cos(��)
    return mat;
}

// �� ��ȯ
Vec3 transformPoint(Vec3 vec, Matrix3x3 mat) {
    Vec3 result;
    result.x = mat.m[0][0] * vec.x + mat.m[0][1] * vec.y + mat.m[0][2] * vec.w;
    result.y = mat.m[1][0] * vec.x + mat.m[1][1] * vec.y + mat.m[1][2] * vec.w;
    result.w = vec.w; // w�� �״�� ����
    return result;
}

// �ܼ� ȭ�� �����
void clearScreen() {
    system("cls");
}

// ȭ�� �׸���
void draw(int angle) {
    clearScreen();

    // P�� ��ġ (����)
    int pX = WIDTH / 2;
    int pY = HEIGHT / 2;

    // O�� �ʱ� ��ġ (P���� �Ÿ�)
    float radiusO = 5.0f; // O�� ȸ�� �ݰ�
    Vec3 o = { radiusO, 0.0f, 1.0f }; // O�� ����� ��ġ

    // Q�� �ʱ� ��ġ (O���� �Ÿ�)
    float radiusQ = 2.0f; // Q�� ȸ�� �ݰ�
    Vec3 q = { radiusQ, 0.0f, 1.0f }; // Q�� ����� ��ġ

    // O�� ȸ��
    float oAngle = angle; // O�� ȸ�� ����
    Vec3 oTransformed;
    oTransformed.x = pX + radiusO * cos(oAngle * M_PI / 180);
    oTransformed.y = pY + radiusO * sin(oAngle * M_PI / 180);
    oTransformed.w = 1.0f; // w�� �״�� ����

    // Q�� ȸ�� (O�� ��ġ�� �߽�����)
    float qAngle = angle * 2; // Q�� O�� �������� �� ������ ȸ��
    Vec3 qTransformed;
    qTransformed.x = oTransformed.x + radiusQ * cos(qAngle * M_PI / 180);
    qTransformed.y = oTransformed.y + radiusQ * sin(qAngle * M_PI / 180);
    qTransformed.w = 1.0f; // w�� �״�� ����

    // ȭ�� �׸���
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == pX && y == pY) {
                printf("P"); // P ���
            }
            else if ((int)(oTransformed.x) == x && (int)(oTransformed.y) == y) {
                printf("O"); // O ���
            }
            else if ((int)(qTransformed.x) == x && (int)(qTransformed.y) == y) {
                printf("Q"); // Q ���
            }
            else {
                printf(" "); // �� ���� ���
            }
        }
        printf("\n");
    }
}

int main() {
    for (int angle = 0; angle < 360; angle += 5) {
        draw(angle); // ȸ�� ������ ���� �׸���
        Sleep(100); // 0.1�� ���
    }
    return 0;
}
