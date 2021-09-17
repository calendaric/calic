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
    keepRunning_ = 0;
    printf("\b\r");
    printf("    ");
}

static void drawTimebar(const struct tm *timeinfo, char *buffer, size_t len)
{
    strftime(buffer, len, "%d.%m.%Y   %X", timeinfo);
    printf("%s\n\r", buffer);
    fflush(stdout);
}

static void drawCalendar(const struct tm *timeinfo)
{
    const Time currentTime = convertTime(timeinfo);
    Calendar calendar = createCalendar(&currentTime);
    const uint8_t weeksCount = sizeof(calendar.week) / sizeof(calendar.week[0]);
    int needBold = 0;
    printf("%3s%3s%3s%3s%3s%3s%3s", "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su");
    printf("\n\r");

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
            if (d == 1 && needBold == 0)
            {
                needBold = 1;
                printf("\033[1m");
            }
            if (j == sizeof(calendar.week[i]) - 1 && needBold == 1)
            {
                needBold = 2;
            }
            if (d == 1 && needBold == 2)
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
    set_conio_terminal_mode();

    char buffer[80];
    printf("\033[?25l");
    while (keepRunning_)
    {
        int keyboard_input = kbhit();
        if (keyboard_input)
        {
            int c = getch();
            if (c == 'x' || c == 26 || c == 3 || c == 4)
            {
                // printf("                                                                  \n\r");
                printf("\n\r");
                printf("\033[");
                printf("%d", 2);

                printf("A");
                break;
            }
        }
        else
        {
            time_t now = time(0);
            struct tm timeinfo;
            timeinfo = *localtime(&now);

            drawTimebar(&timeinfo, buffer, sizeof(buffer));
            drawCalendar(&timeinfo);

            printf("\033[");
            printf("%d", 8);
            printf("A");
            usleep(20 * 1000);
        }
    }

    for (int i = 0; i < 9; ++i)
    {
        printf("                                                                  \n\r");
    }

    printf("\b\r");
    printf("\033[");
    printf("%d", 9);
    printf("A");

    return 0;
}
