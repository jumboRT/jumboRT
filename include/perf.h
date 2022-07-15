#ifndef PERF_H
# define PERF_H

#include <sys/time.h>

typedef struct s_perf	t_perf;

struct s_perf {
	struct timeval	tv;
};

void	perf_start(t_perf *perf);
void	perf_split(t_perf *perf, const char *name);

#endif
