#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    int8_t week[6][7];
    int8_t weekNumber;
    int8_t currentDayNumber;
} Calendar;


typedef enum { January = 1, Febrary = 2, March = 3, April = 4, May = 5, June = 6, July = 7, August = 8, September = 9, October = 10, November = 11, December = 12 } Month;
typedef enum { Monday = 1, Tuesday = 2, Wednesday = 3, Thursday = 4, Friday = 5, Saturday = 6, Sunday = 7 } WeekDay;

typedef struct {
    int seconds;
    int minutes;
    int hours;
    int monthDay;
    Month month;
    int year;
    WeekDay weekDay;
    int yearDay;
} Time;

Time convertTime(const struct tm* tm);

Calendar createCalendar(const Time* currentTime);
