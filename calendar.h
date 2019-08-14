#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef struct Calendar_s {
    uint8_t week[6][7];
    struct {
        uint8_t weekNumber;
        uint8_t currentDayNumber;
    } aux;
} Calendar;

typedef enum Month_e { January = 1, Febrary = 2, March = 3, April = 4, May = 5, June = 6, July = 7, August = 8, September = 9, October = 10, November = 11, December = 12 } Month;
typedef enum WeekDay_e {
    Monday = 1, Tuesday = 2, Wednesday = 3, Thursday = 4, Friday = 5, Saturday = 6, Sunday = 7 } WeekDay;

typedef struct Time_s {
    int seconds;
    int minutes;
    int hours;
    int monthDay;
    Month month;
    int year;
    WeekDay weekDay;
    int yearDay;
} Time;

bool isLeap(int year);
int getNumberOfDays(int month, int year);
void getYearAndWeek(struct tm TM, int* YYYY, int* WW);
Calendar calendar(void);
void drawTimebar(char* buffer, size_t len);
void interruptHandler(int);
