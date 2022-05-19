#include "rt.h"
#include <stdlib.h>

int
	rt_exit(void *ctx)
{
	thread_stop(ctx);
	exit(EXIT_SUCCESS);
}
