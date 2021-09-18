#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "calendar.h"
#include <sys/select.h>
#include <termios.h>

struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int getch()
{
    int r = 0;
    unsigned char c = 0;
    if ((r = read(0, &c, sizeof(c))) < 0)
    {
        return r;
    }
    else
    {
        return c;
    }
}

static volatile sig_atomic_t keepRunning_ = 1;

static void interruptHandler(int n)
{
    (void)n;
    __sync_fetch_and_sub(&keepRunning_, 1);
    printf("\b\r");
    printf("                                  \r");
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
    int needBold = 0;
    printf("%3s%3s%3s%3s%3s%3s%3s", "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su");
    printf("\n\r");
    size_t month_counter = 0;

    for (uint8_t i = 0; i < weeksCount; ++i)
    {
        for (uint8_t j = 0; j < sizeof(calendar.week[i]); ++j)
        {
            const uint8_t d = calendar.week[i][j];
            if (d == calendar.currentDayNumber)
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

int main(void)
{

    
    signal(SIGINT, *interruptHandler);
    signal(SIGHUP, *interruptHandler);
    signal(SIGQUIT, *interruptHandler);
    signal(SIGABRT, *interruptHandler);
    set_conio_terminal_mode();

    time_t now = time(0);
    time_t calendar = time(0);
    struct tm timeinfo = *localtime(&now);
    struct tm calendar_info = *localtime(&now);

    char buffer[80];
    int month_cursor = 0;
    printf("\033[?25l");

    while (keepRunning_)
    {
        now = time(0);
        if (month_cursor == 0)
        {
            calendar_info = timeinfo;
        }

        int keyboard_input = kbhit();

        if (keyboard_input)
        {
            int c = getch();
            enum
            {
                CtrlC = 3,
                CtrlD = 4,
                CtrlZ = 26,
                Esc = 27
            };
            if (c == CtrlC || c == CtrlC || c == CtrlZ)
            {
                printf("\n\r");
                printf("\033[");
                printf("%d", 2);

                printf("A");
                break;
            }
            else if (c == Esc)
            {
                int _ = getch();
                switch (getch())
                {
                case 'D':
                    month_cursor -= 1;
                    sub_one_month(&calendar_info);
                    break;
                case 'C':
                    month_cursor += 1;
                    add_one_month(&calendar_info);
                default:
                    break;
                }
            }
        }
        timeinfo = *localtime(&now);

        if (month_cursor != 0)
        {
            timeinfo.tm_mon = calendar_info.tm_mon;
        }
        void shift_month(time_t * now, struct tm * current, int month_cursor);
        drawTimebar(&timeinfo, buffer, sizeof(buffer));
        drawCalendar(&calendar_info);

        printf("\033[");
        printf("%d", 8);
        printf("A");
        usleep(20 * 1000);
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

