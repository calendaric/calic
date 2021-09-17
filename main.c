#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "calendar.h"

static volatile sig_atomic_t keepRunning_ = 1;

static void interruptHandler(int n) {
    (void)n;
    keepRunning_ = 0;
    printf("\b\r");
    printf("    ");
}

static void drawTimebar(const struct tm* timeinfo, char* buffer, size_t len) {
    strftime(buffer, len, "%d.%m.%Y   %X", timeinfo);
    puts(buffer);
}

static void drawCalendar(const struct tm* timeinfo) {
    const Time currentTime  = convertTime(timeinfo);
    Calendar calendar = createCalendar(&currentTime);
    const uint8_t weeksCount = sizeof(calendar.week) / sizeof(calendar.week[0]);
    int needBold = 0;
    printf("%3s%3s%3s%3s%3s%3s%3s", "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su");
    printf("\n");

    for (uint8_t i = 0; i < weeksCount; ++i) {
        for (uint8_t j = 0; j < sizeof(calendar.week[i]); ++j) {
            const uint8_t d = calendar.week[i][j];
            if (d == calendar.currentDayNumber) {
                printf(" \033[7m");
                printf("%*d", 2, d);
                printf("\033[27m");
                continue;
            }
            if (d == 1 && needBold == 0) {
                needBold = 1;
                printf("\033[1m");
            }
            if (j == sizeof(calendar.week[i]) - 1 && needBold == 1) {
                needBold = 2;
            }
            if (d == 1 && needBold == 2) {
                printf("\033[0m");
            }
            printf("%3d", d);
        }
        printf("\n");
    }
}

int main(void) {
    signal(SIGINT, *interruptHandler);

    char buffer[80];

    while (keepRunning_) {
        printf("\e[1;1H\e[2J");
        time_t now = time(0);
        struct tm timeinfo;
        timeinfo = *localtime(&now);
        drawTimebar(&timeinfo, buffer, sizeof(buffer));
        drawCalendar(&timeinfo);
        usleep(100000);

        printf("\b\r");
        printf("\033[");
        printf("%d", 8);
        printf("A");
    }

    for (int i = 0; i < 8; ++i) {
        printf("                                                                  \n");
    }

    printf("\b\r");
    printf("\033[");
    printf("%d", 8);
    printf("A");

    return 0;
}
