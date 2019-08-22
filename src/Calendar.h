#ifndef CALENDAR_H
#define CALENDAR_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "Time.h"

typedef struct {
    int8_t week[6][7];
    int8_t weekNumber;
    int8_t currentDayNumber;
} Calendar;

Calendar createCalendar(const Time* currentTime);

#endif // CALENDAR_H
