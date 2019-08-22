#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "CalendarView.h"
#include <termios.h>


static struct termios initial_settings;
static struct termios new_settings;

enum Keys { Escape = 27, LeftArrow = 68, RightArrow = 67 };
static volatile sig_atomic_t keepRunning_ = 1;

void interruptHandler(int n) {
    (void)n;
    keepRunning_ = 0;
    printf("\b\r");
    printf("    ");
}

void drawTimebar(time_t rawtime) {
    printf("%s", ctime(&rawtime));
}

int main(void) {
    printf("\e[?25l");
    signal(SIGINT, *interruptHandler);

    unsigned char key;
    int counter = 0;
    int n;
    tcgetattr(0, &initial_settings);

    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_cc[VMIN] = 0;
    new_settings.c_cc[VTIME] = 0;

    tcsetattr(0, TCSANOW, &new_settings);

    struct tm timeinfo;
    Time currentTime = convertTime(&timeinfo);
    int monthShift = 0;

    while (keepRunning_) {
        counter++;
        time_t now = time(0);
        drawTimebar(now);
        if (monthShift == 0) {
            timeinfo = *localtime(&now);
        }
        else {
            mktime(&timeinfo);
        }
        currentTime = convertTime(&timeinfo);

        show(&currentTime);
        usleep(10000);
        printf("\b\r");
        fflush(stdout);
        printf("\033[");printf("%d", 8); printf("A");
        n = getchar();
        if (n != EOF) {
            key = n;
            if (key == RightArrow) {
                monthShift++;
                timeinfo.tm_mon += 1;
            }
            else if (key == LeftArrow) {
                monthShift--;
                timeinfo.tm_mon -= 1;
            }
        }
    }
    for (int i = 0; i < 8; ++i) {
        printf("                                                                  \n");
    }

    printf("\b\r");
    printf("\033[");printf("%d", 8); printf("A");
    tcsetattr(0, TCSANOW, &initial_settings);
    printf("\e[?25h");
    return 0;
}

