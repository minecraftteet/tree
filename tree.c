#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#endif

void printTree(int height, int width) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < (width / 2) - i; j++)
            printf(" ");

        for (int k = 0; k < 2 * i + 1; k++)
            printf("*");

        printf("\n");
    }

    // Print tree trunk
    for (int i = 0; i < (width / 2) - 1; i++)
        printf(" ");

    printf("|\n");
}

void printSnow(int width, int height) {
    for (int i = 0; i < 20; i++) {
        int x = rand() % width;
        int y = rand() % height;

        printf("\033[%d;%dH*\n", y, x);
    }
}

void printPresents(int width, int height) {
    int presentX = width / 2;
    int presentY = height - 2;

    printf("\033[%d;%dH", presentY, presentX);
    printf("Presents!!!\n");
}

void printSunOrMoon(int width, int height, int hour) {
    int centerX = width / 2;
    int centerY = abs(sin((hour - 12) * M_PI / 12) * (height / 2)) + 1;

    if (hour >= 6 && hour < 18) {
        // Daytime, print sun
        printf("\033[%d;%dH", centerY, centerX);
        printf("☀\n");
    } else {
        // Nighttime, print moon
        printf("\033[%d;%dH", centerY, centerX);
        printf("🌙\n");
    }
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J");
#endif
}

void getTerminalSize(int *width, int *height) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    *width = ws.ws_col;
    *height = ws.ws_row;
#endif
}

int main() {
    srand(time(NULL));

    int width, height;
    getTerminalSize(&width, &height);

    time_t t;
    struct tm *tm_info;

    while (1) {
        time(&t);
        tm_info = localtime(&t);

        clearScreen();
        getTerminalSize(&width, &height);

        int treeHeight = height / 2;  // Adjust the height of the tree based on terminal size
        printSunOrMoon(width, height, tm_info->tm_hour);
        printTree(treeHeight, width);
        printSnow(width, height);

        if ((tm_info->tm_mday == 25 && tm_info->tm_mon == 11) || (tm_info->tm_mday == 26 && tm_info->tm_hour == 0 && tm_info->tm_min == 0)) {
            printPresents(width, height);
        }

        usleep(200000);
    }

    return 0;
}
