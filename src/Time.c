

#include "Time.h"

Time convertTime(const struct tm* tm) {
    Time time;
    time.seconds = tm->tm_sec;
    time.minutes = tm->tm_min;
    time.hours = tm->tm_hour;
    time.monthDay = tm->tm_mday;
    time.month = (Month)(tm->tm_mon + 1);
    time.year = tm->tm_year + 1900;

    if (tm->tm_wday == 0) {
        time.weekDay = Sunday;
    }
    else {
        time.weekDay = (WeekDay)tm->tm_wday;
    }

    time.yearDay = tm->tm_year;
    return time;
}

void changeMonth(Time* time, int offset) {
    time->month += offset;
    time->monthDay = 1;
    if (time->month > December) {
        time->month = time->month - December;
        time->year += 1;
    }
    else if (time->month < January) {
        time->month = December - time->month;
        time->year -= 1;
    }
}

