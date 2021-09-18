#include "calendar.h"

bool isLeap(int year)
{
    if (year % 4 == 0)
    {
        if (year % 100 == 0 && year % 400 != 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    return false;
}

int getNumberOfDays(int month, int year)
{
    switch (month)
    {
    case February:
        return (isLeap(year) ? 29 : 28);
    case April:
    case June:
    case September:
    case November:
        return 30;
    default:
        return 31;
    }
}

static WeekDay weekDayInMonthBegin(int currentMonthDay, WeekDay currentWeekDay)
{
    while (currentMonthDay > 1)
    {
        --currentMonthDay;
        --currentWeekDay;
        if (currentWeekDay < Monday)
        {
            currentWeekDay = Sunday;
        }
    }
    return currentWeekDay;
}

static void fillCurrentMonth(Calendar *calendar, size_t weekIndex, size_t weekDayIndex, size_t daysCountInCurrentMonth)
{
    const size_t rowCount = sizeof(calendar->week) / sizeof(calendar->week[0]);
    const size_t colCount = sizeof(calendar->week[0]);
    size_t n = 1;
    for (size_t row = weekIndex; row < rowCount; ++row)
    {
        for (size_t col = weekDayIndex; col < colCount; ++col)
        {
            if (n > daysCountInCurrentMonth)
            {
                n = 1;
            }
            if (col == 6)
            {
                weekDayIndex = 0;
            }
            calendar->week[row][col] = (int8_t)(n);
            ++n;
        }
    }
}

static void fillPreviousMonth(Calendar *calendar, size_t weekIndex, size_t weekDayIndex, size_t daysCountInPreviousMonth)
{
    // offset to last days of previous month
    if (weekIndex == 1)
    {
        weekIndex = 0;
        weekDayIndex = 6;
    }
    else
    {
        --weekDayIndex;
    }

    int n = (int)daysCountInPreviousMonth;
    for (int row = (int)weekIndex; row >= 0; --row)
    {
        for (int col = (int)weekDayIndex; col >= 0; --col)
        {
            calendar->week[row][col] = (int8_t)(n);
            --n;
        }
    }
}

Time convertTime(const struct tm *tm)
{
    Time time;
    time.seconds = tm->tm_sec;
    time.minutes = tm->tm_min;
    time.hours = tm->tm_hour;
    time.monthDay = tm->tm_mday;
    time.month = (Month)(tm->tm_mon + 1);
    time.year = tm->tm_year + 1900;

    if (tm->tm_wday == 0)
    {
        time.weekDay = Sunday;
    }
    else
    {
        time.weekDay = (WeekDay)tm->tm_wday;
    }

    time.yearDay = tm->tm_year;
    return time;
}

Calendar createCalendar(const Time *currentTime)
{
    Calendar calendar = {.weekNumber = 0, .currentDayNumber = 0, .week = {{0}}};
    calendar.currentDayNumber = (int8_t)currentTime->monthDay;

    const WeekDay weekDayFirst = weekDayInMonthBegin(currentTime->monthDay, currentTime->weekDay);
    const uint8_t daysCountInCurrentMonth = (uint8_t)getNumberOfDays(currentTime->month, currentTime->year);
    const uint8_t daysCountInPreviousMonth = (uint8_t)getNumberOfDays(currentTime->month - 1, currentTime->year);

    size_t dayOfWeekIndex = 0;
    size_t weekInMonthIndex = 0;
    if (weekDayFirst == Monday)
    {
        weekInMonthIndex = 1;
    }
    else
    {
        dayOfWeekIndex = weekDayFirst - 1;
    }

    fillCurrentMonth(&calendar, weekInMonthIndex, dayOfWeekIndex, daysCountInCurrentMonth);
    fillPreviousMonth(&calendar, weekInMonthIndex, dayOfWeekIndex, daysCountInPreviousMonth);

    return calendar;
}

int dow(int day, int month, int year)
{
    return (day + ((153 * (month + 12 * ((14 - month) / 12) - 3) + 2) / 5) + (365 * (year + 4800 - ((14 - month) / 12))) + ((year + 4800 - ((14 - month) / 12)) / 4) - ((year + 4800 - ((14 - month) / 12)) / 100) + ((year + 4800 - ((14 - month) / 12)) / 400) - 32045) % 7;
}

void sub_one_month(struct tm *timeinfo)
{
    if (timeinfo->tm_mon == 0)
    {
        timeinfo->tm_year -= 1;
        timeinfo->tm_mon = 11;
    }
    else
    {
        timeinfo->tm_mon -= 1;
    }

    timeinfo->tm_wday = dow(timeinfo->tm_mday + 1, timeinfo->tm_mon + 1, 1900 + timeinfo->tm_year);
}

void add_one_month(struct tm *timeinfo)
{
    if (timeinfo->tm_mon == 11)
    {
        timeinfo->tm_mon = 0;
        timeinfo->tm_year += 1;
    }
    else
    {
        timeinfo->tm_mon += 1;
    }

    timeinfo->tm_wday = dow(timeinfo->tm_mday + 1, timeinfo->tm_mon + 1, 1900 + timeinfo->tm_year);
}