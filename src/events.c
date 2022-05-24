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
	{
		mutex_lock(&state->mtx);
		thread_reset(ctx);
		mutex_unlock(&state->mtx);
	}
	if (key == RT_KEY_P)
	{
		mutex_lock(&state->mtx);
		state->use_conic = !state->use_conic;
		thread_reset(ctx);
		mutex_unlock(&state->mtx);
	}
	return (0);
}

