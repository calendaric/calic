#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "calendar.h"
#include "compatibility.h"

static volatile sig_atomic_t keepRunning_ = 1;

static void interruptHandler(int n)
{
    printf("Received signal =%d", n);
    atomic_zeroing(&keepRunning_);
}

static void drawTimebar(const struct tm *timeinfo, char *buffer, size_t len)
{
    strftime(buffer, len, "%d.%m.%Y   %X", timeinfo);
    printf("%s\n\r", buffer);
}

static void drawCalendar(const struct tm *timeinfo)
{
    const Time currentTime = convertTime(timeinfo);
    Calendar calendar = createCalendar(&currentTime);
    const uint8_t weeksCount = sizeof(calendar.week) / sizeof(calendar.week[0]);
    printf("%3s%3s%3s%3s%3s%3s%3s", "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su");
    printf("\n\r");
    size_t month_counter = 0;

    for (uint8_t i = 0; i < weeksCount; ++i)
    {
        for (uint8_t j = 0; j < sizeof(calendar.week[i]); ++j)
        {
            const uint8_t d = calendar.week[i][j];
            if (d == calendar.currentDayNumber && month_counter == 1)
            {
                printf(" \033[7m");
                printf("%*d", 2, d);
                printf("\033[27m");
                continue;
            }
            if (d == 1 && month_counter == 0)
            {
                month_counter++;
                printf("\033[1m");
            }
            else if (d == 1 && month_counter == 1)
            {
                month_counter++;
            }
            if (month_counter == 1)
            {
                printf("\033[1m");
            }
            else
            {
                printf("\033[0m");
            }
            printf("%3d", d);
        }
        printf("\n\r");
    }
}

int help()
{
    printf("calic - \033[1mcal\033[0mendar \033[1mi\033[0mntera\033[1mc\033[0mtive\n");
    printf("\n");
    printf("Usage:\n");
    printf("    left / right arrows - scroll previous or next month\n");
    printf("    'q'                 - quit\n");
    printf("\n");
    printf("License: MIT\n");
    printf("Sources: https://github.com/calendaric/calic\n");
    printf("2021 Ivan Azoyan <ivan.azoyan@gmail.com>\n");
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))
    {
        return help();
    }

    signal(SIGINT, *interruptHandler);
    signal(SIGABRT, *interruptHandler);

    set_conio_terminal_mode();

    time_t now = time(0);

    struct tm timeinfo = local_time(&now);
    struct tm calendar_info = local_time(&now);

    char buffer[80];
    int month_cursor = 0;
    printf("\033[?25l");

    int request_time_counter = 0;

    while (keepRunning_)
    {
        if (request_time_counter == 0)
        {
            now = time(0);
        }
        if (month_cursor == 0)
        {
            calendar_info = timeinfo;
        }

        int keyboard_input = keyboard_hit();

        if (keyboard_input)
        {
            request_time_counter = 0;
            now = time(0);
            int c = get_char();

            if (c == CtrlC || c == CtrlD || c == CtrlZ || c == 'q')
            {
                printf("\n\r");
                printf("\033[");
                printf("%d", 2);

                printf("A");
                break;
            }

            switch (c)
            {
            case Left:
            case Up:
                month_cursor -= 1;
                sub_one_month(&calendar_info);
                break;
            case Right:
            case Down:
                month_cursor += 1;
                add_one_month(&calendar_info);
            default:
                break;
            }
        }
        if (request_time_counter == 0)
        {
            timeinfo = local_time(&now);

            if (month_cursor != 0)
            {
                timeinfo.tm_mon = calendar_info.tm_mon;
                timeinfo.tm_year = calendar_info.tm_year;
            }

            drawTimebar(&timeinfo, buffer, sizeof(buffer));
            drawCalendar(&calendar_info);

            printf("\033[");
            printf("%d", 8);
            printf("A");
        }
        const int sleep_time = 20 * 1000;
        request_time_counter += sleep_time;
        microsec_sleep(sleep_time);
        request_time_counter %= 1000000;
    }

    for (int i = 0; i < 9; ++i)
    {
        printf("                                                                  \n\r");
    }

    printf("\b\r");
    printf("\033[");
    printf("%d", 9);
    printf("A");
    printf("\033[?25h");
    return 0;
}
