#include "perf.h"


#include <stdlib.h>
#include <ft_printf.h>

void
	perf_start(t_perf *perf)
{
	gettimeofday(&perf->tv, NULL);
}

void
	perf_split(t_perf *perf, const char *name)
{
	struct timeval	tv;

	tv = perf->tv;
	perf_start(perf);
	tv.tv_sec = perf->tv.tv_sec - tv.tv_sec;
	if (perf->tv.tv_usec < tv.tv_usec)
	{
		tv.tv_usec = perf->tv.tv_usec - tv.tv_usec + 1000000;
		tv.tv_sec -= 1;
	}
	else
		tv.tv_usec = perf->tv.tv_usec - tv.tv_usec;
	ft_printf("perf: %s: %u.%03u\n", name, tv.tv_sec, tv.tv_usec / 1000);
}

