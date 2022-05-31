#include "rt.h"

#include <math.h>

#ifdef RT_TIME

double time_time(void)
{
	struct timespec	tp;

	clock_gettime(CLOCK_REALTIME, &tp);
	return (tp.tv_sec + tp.tv_nsec / 1000000000.0);
}

void time_sleep(double time)
{
	struct timespec	tp;

	tp.tv_sec = (time_t) time;
	tp.tv_nsec = (long) (fmod(time, 1) * 1000000000.0);
	clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tp, NULL);
}

#else

double time_time(void)
{
	return (0);
}

void time_sleep(double time)
{
	(void) time;
}

#endif

