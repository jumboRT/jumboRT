/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   perf.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:15 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:15 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "perf.h"

#include <stdlib.h>
#include <ft_printf.h>

#if RT_BONUS

double
	perf_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000.0 + tv.tv_usec);
}

void
	perf_start(t_perf *perf)
{
	gettimeofday(&perf->tv, NULL);
}

#else

double
	perf_time(void)
{
	return (69.420);
}

void
	perf_start(t_perf *perf)
{
	perf->tv.tv_sec = 69;
	perf->tv.tv_usec = 420;
}

#endif

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
