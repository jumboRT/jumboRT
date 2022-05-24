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
	t_rt_state	*state;

	state = ctx;
	ft_printf("key %d\n", key);
	if (key == RT_KEY_ESC || key == RT_KEY_Q)
		rt_exit(ctx);
	if (key == RT_KEY_R)
		thread_reset(ctx, state->use_conic);
	if (key == RT_KEY_P)
		thread_reset(ctx, !state->use_conic);
	return (0);
}

