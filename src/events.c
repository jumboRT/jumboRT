#include "rt.h"
#include <stdlib.h>
#include <ft_printf.h>

int
	rt_exit(void *ctx)
{
	thread_stop(ctx);
	exit(EXIT_SUCCESS);
	return (0);
}

int
	rt_key_down(int key, void *ctx)
{
	ft_printf("key %d\n", key);
	if (key == 53 || key == 12)
		rt_exit(ctx);
	if (key == 15)
		thread_reset(ctx);
	return (0);
}

