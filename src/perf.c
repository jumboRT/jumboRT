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
	timersub(&perf->tv, &tv, &tv);
	ft_printf("perf: %s: %u.%03u\n", name, tv.tv_sec, tv.tv_usec / 1000);
}

