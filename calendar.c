#include "calendar.h"


bool isLeap(int year) {
    if (year % 4 == 0) {
        if (year % 100 == 0 && year % 400 != 0) {
            return false;
        } else {
            return true;
        }
    }
    return false;
}

int getNumberOfDays(int month, int year) {
    switch (month) {
    case Febrary :
        return (isLeap(year) ? 29 : 28);
    case April :
    case June :
    case September :
    case November :
        return 30;
    default :
        return 31;
    }
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

int weekDayInMonthBegin(int currentMday, int currentWday) {
    while (currentMday --> Monday) {
        --currentWday;
        if (currentWday < Monday) {
            currentWday = Sunday;
        }
    }
    return currentWday;
}

Time convertTime(struct tm tm) {
    Time time = {0};
    time.seconds = tm.tm_sec;
    time.minutes = tm.tm_min;
    time.hours = tm.tm_hour;
    time.monthDay = tm.tm_mday;
    time.month = tm.tm_mon + 1;
    time.year = tm.tm_year + 1900;

    if (tm.tm_wday == 0) {
        time.weekDay = Sunday;
    }
    else {
        time.weekDay = tm.tm_wday;
    }

    time.yearDay = tm.tm_year;
    return time;
}

void fillCurrentMonth(Calendar* calendar, size_t weekIndex, size_t weekDayIndex, size_t daysCountInCurrentMonth) {
    const size_t rowCount = sizeof(calendar->week) / sizeof (calendar->week[0]);
    const size_t colCount = sizeof(calendar->week[0]);
    size_t n = 1;
    for (size_t row = weekIndex; row < rowCount; ++row) {
        for (size_t col = weekDayIndex; col < colCount; ++col) {
            if (n > daysCountInCurrentMonth) {
                n = 1;
            }
            calendar->week[row][col] = n;
            n++;
            if (col == 6) weekDayIndex = 0;
        }
    }
}

void fillPreviousMonth(Calendar* calendar, int weekIndex, int weekDayIndex, size_t daysCountInPreviousMonth) {
    // offset to last days of previous month
    if (weekIndex == 1) {
        weekIndex = 0; weekDayIndex = 6;
    }
    else {
        weekDayIndex = weekDayIndex - 1;
    }

    size_t n = daysCountInPreviousMonth;
    for (int row = weekIndex; row >= 0; --row) {
        for (int col = weekDayIndex; col >= 0; --col) {
            calendar->week[row][col] = n;
            n--;
        }
    }
}

Calendar calendar() {
    time_t     now = time(0);
    struct tm  tm;
    char       buf[80];
    tm = *localtime(&now);
    Time currentTime = convertTime(tm);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tm);
    int week = 0;
    Calendar calendar = { .week = {{0}}, { .weekNumber = 0, .currentDayNumber = 0} };
    calendar.aux.currentDayNumber = currentTime.monthDay;
    calendar.aux.weekNumber = (uint8_t)week;

    const WeekDay weekDayFirst = weekDayInMonthBegin(currentTime.monthDay, currentTime.weekDay);
    const uint8_t daysCountInCurrentMonth = (uint8_t)getNumberOfDays(currentTime.month, currentTime.year);
    const uint8_t daysCountInPreviousMonth = (uint8_t)getNumberOfDays(currentTime.month - 1, currentTime.year);

    size_t dayOfWeekIndex = 0;
    size_t weekInMonthIndex = 0;
    if (weekDayFirst == Monday) {
        weekInMonthIndex = 1;
    }
    else {
        dayOfWeekIndex = weekDayFirst - 1;
    }

    fillCurrentMonth(&calendar, weekInMonthIndex, dayOfWeekIndex, daysCountInCurrentMonth);
    fillPreviousMonth(&calendar, weekInMonthIndex, dayOfWeekIndex, daysCountInPreviousMonth);

    return calendar;
}


