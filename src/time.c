#include "rt.h"

#include <math.h>

#ifdef RT_TIME

double time_time(void)
{
	struct timespec	tp;

	clock_gettime(CLOCK_REALTIME, &tp);
	return ((double) tp.tv_sec + ((double) tp.tv_nsec / 1000000000.0));
}

void time_sleep(double time)
{
	struct timespec	tp;

	tp.tv_sec = (time_t) time;
	tp.tv_nsec = (long) (fmod(time, 1) * 1000000000.0);
#ifdef RT_LINUX
	clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tp, NULL);
#else
	nanosleep(&tp, NULL);
#endif
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

