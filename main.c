#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>

static volatile int keepRunning = 1;

bool isLeap(int year) {
    if (year % 4 == 0) {
        if ( year % 100 == 0 && year % 400 != 0 ) {
            return false;
        } else return true;
    }
    return false;
}

int getNumberOfDays(int month, int year) {
    if(month == 2) {
        if (isLeap(year)) return 29;
        else return 28;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    else return 31;
}

void getYearAndWeek(struct tm TM, int* YYYY, int* WW) {
    *YYYY = TM.tm_year + 1900;
    int day = TM.tm_yday;

    int Monday = day - ( TM.tm_wday + 6 ) % 7;
    int MondayYear = 1 + ( Monday + 6 ) % 7;
    int Monday01 = ( MondayYear > 4) ? MondayYear - 7 : MondayYear;
    *WW = 1 + ( Monday - Monday01 ) / 7;

    if (*WW == 0) {
        (*YYYY)--;
        *WW = 52;
        if (MondayYear == 3 || MondayYear == 4 || (isLeap(*YYYY) && MondayYear == 2)) *WW = 53;
    }

    if (*WW == 53) {
        int daysInYear = isLeap(*YYYY) ? 366 : 365;
        if (daysInYear - Monday < 3) {
            (*YYYY)++;
            *WW = 1;
        }
    }
}

struct Calendar {
    uint8_t week[6][7];
    struct {
        uint8_t weekNumber;
        uint8_t current_day_number;
    } aux;
};

struct Calendar calendar() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    int month = tstruct.tm_mon + 1;
    int year = tstruct.tm_year + 1900;
    int week = 0;
    struct Calendar calendar;
    memset(&calendar, 0, sizeof(calendar));
    calendar.aux.current_day_number = (uint8_t)tstruct.tm_mday;
    tstruct.tm_mday = 1;
    getYearAndWeek(tstruct, &year, &week);

    calendar.aux.weekNumber = (uint8_t)week;

    const uint8_t weeks_count = sizeof(calendar.week) / sizeof(calendar.week[0]);
    const uint8_t days_count = (uint8_t)getNumberOfDays(month - 1, year);
    uint8_t current = 1;
    for (uint8_t i = 0; i < weeks_count; ++i) {
        for (uint8_t j = 0; j < sizeof(calendar.week[0]); ++j) {
            uint8_t d = (days_count - 7 + tstruct.tm_wday + current++ - 2) % days_count;
            calendar.week[i][j] = d != 0 ? d : days_count;
        }
    }
    return calendar;
}

void intHandler(int n) {
    keepRunning = 0;
    printf("\b\r");
    printf("  ");
}

int main(void) {
    signal(SIGINT, intHandler);
    char buffer[80];
    while(keepRunning) {
        time_t rawtime;
        struct tm * timeinfo;
        time (&rawtime);
        timeinfo = localtime (&rawtime);

        strftime (buffer, 80,"%d.%m.%Y   %X",timeinfo);
        puts (buffer);
        struct Calendar calic = calendar();
        const uint8_t weeks_count = sizeof(calic.week) / sizeof(calic.week[0]);
        printf("%3s%3s%3s%3s%3s%3s%3s", "mo", "tu", "we", "th", "fr", "sa", "su");
        printf("\n");
        int needBold = 0;
        for (uint8_t i = 0; i < weeks_count; ++i) {
            for (uint8_t j = 0; j < sizeof(calic.week[i]); ++j) {
                const uint8_t d = calic.week[i][j];
                if (d == calic.aux.current_day_number) {
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
        sleep(1);
        printf("\b\r");
        fflush(stdout);
        printf("\033[");printf("%d", 8); printf("A");
    }
    for (int i = 0; i < 8; ++i) printf("                                                                  \n");
    printf("\b\r");
    printf("\033[");printf("%d", 8); printf("A");
    return 0;
}
