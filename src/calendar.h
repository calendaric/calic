#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef struct Calendar_s {
    int8_t week[6][7];
    int8_t weekNumber;
    int8_t currentDayNumber;
} Calendar;

Calendar createCalendar(const struct tm* current_timeinfo);
