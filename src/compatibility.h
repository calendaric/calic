#pragma once

#include <string.h>
#include <stdlib.h>

#if defined(__WIN32__) || defined(__WIN64__) || defined(__CYGWIN32__) || defined(__CYGWIN64__) || defined(_MSVC_TRADITIONAL)
#include <conio.h>
#include <Windows.h>

struct tm* local_time(time_t *const time) {
	struct tm timeinfo;
	localtime_s(&timeinfo, time);
	return &timeinfo;
}

#include <windows.h>

enum KeyCodes
{
	CtrlC = 3,
	CtrlD = 4,
	CtrlZ = 26,
	Esc = 27,
	Left = 75,
	Right = 77,
};

void microsec_sleep(int waitTime) {
	__int64 time1 = 0, time2 = 0, freq = 0;

	QueryPerformanceCounter((LARGE_INTEGER*)& time1);
	QueryPerformanceFrequency((LARGE_INTEGER*)& freq);

	do {
		QueryPerformanceCounter((LARGE_INTEGER*)& time2);
	} while ((time2 - time1) < waitTime);
}

int keyboard_hit() {
	return _kbhit();
}

int get_char() {
	return _getch();
}

void atomic_decrement(long int* n) {
	InterlockedDecrement(n);
}


#elif defined(unix)
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

enum KeyCodes
{
	CtrlC = 3,
	CtrlD = 4,
	CtrlZ = 26,
	Esc = 27,
	Left = 68,
	Right = 67,
};

struct termios orig_termios;

void reset_terminal_mode()
{
	tcsetattr(0, TCSANOW, &orig_termios);
}

void microsec_sleep(int microseconds) {
	usleep(microseconds);
}


void atomic_decrement(long int* n) {
	__sync_fetch_and_sub(&keepRunning_, 1);
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

int keyboard_hit()
{
	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(0, &fds);
	return select(1, &fds, NULL, NULL, &tv);
}

int get_char()
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
#endif
