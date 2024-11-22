#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // _kbhit(), _getch() 함수 사용 (Windows)
#include <windows.h> // Sleep() 함수 사용
#include <time.h>   // 랜덤 색상 출력을 위해 필요

// ANSI 색상 코드 정의
const char* colors[] = {
    "\x1b[31m", // 빨강
    "\x1b[32m", // 초록
    "\x1b[33m", // 노랑
    "\x1b[34m", // 파랑
    "\x1b[35m", // 보라
    "\x1b[36m", // 하늘
    "\x1b[37m"  // 흰색
};
int num_colors = sizeof(colors) / sizeof(colors[0]);

void clearScreen() {
    system("cls");  // 콘솔 화면 지우기 (Windows용 명령어)
}

void changeColor() {
    int random_color = rand() % num_colors;
    printf("%s색상이 변경되었습니다!\n", colors[random_color]);
}

int main() {
    char ch;
    srand(time(NULL));  // 랜덤 시드 설정

    printf("S를 눌러 게임을 시작하세요...\n");

    // 'S' 입력 대기
    while ((ch = _getch()) != 'S') {}

    printf("3초 후 게임이 시작됩니다...\n");
    Sleep(3000);  // 3초 대기 (Windows용 Sleep 함수, 밀리초 단위)
    printf("게임이 시작되었습니다!\n");

    // 'Q'를 누를 때까지 색상 변경
    while (1) {
        if (_kbhit()) {  // 키 입력 체크 (비차단)
            ch = _getch();
            if (ch == 'Q') {
                printf("게임이 종료되었습니다.\n");
                break;
            }
        }

        clearScreen();
        changeColor();
        Sleep(1000);  // 1초 대기
    }

    return 0;
}
