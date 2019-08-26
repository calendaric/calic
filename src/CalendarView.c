#include "CalendarView.h"
#include <stdio.h>

void show(const Time* time) {
    Calendar calendar = createCalendar(time);
    const uint8_t weeksCount = sizeof(calendar.week) / sizeof(calendar.week[0]);
    int needBold = 0;

    printf("%3s%3s%3s%3s%3s%3s%3s", "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su");
    printf("\n");

    for (uint8_t i = 0; i < weeksCount; ++i) {
        for (uint8_t j = 0; j < sizeof(calendar.week[i]); ++j) {
            const uint8_t d = calendar.week[i][j];
            if (d == calendar.currentDayNumber) {
                printf(" \033[7m");
                printf("%*d", 2,d);
                printf("\033[27m");
                continue;
            }
            if (d == 1 && needBold == 0) {
                needBold = 1;
                printf("\033[1m");
            }
            if (d == 31 && needBold == 1) {
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
