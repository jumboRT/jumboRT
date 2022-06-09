#include "rt.h"

#include "ft_printf.h"

static void
	thread_render(t_thread_ctx *ctx, t_rt_state *state)
{
	size_t	begin;
	size_t	end;
	long	version;
	t_vec	colors[RT_RENDER_CHUNK_SIZE];
	double	now;
	double	sps;

	while (state->idx < state->end && state->running)
	{
		begin = state->idx;
		end = begin + RT_RENDER_CHUNK_SIZE;
		if (end >= state->end)
			end = state->end;
		state->idx = end;
		version = state->version;
		mutex_unlock(&state->mtx);
		render_range(ctx, state, colors, begin, end);
		mutex_lock(&state->mtx);
		if (version >= state->version)
		{
			now = time_time();
			if (RT_TIME_PROGRESS && now > state->last_update + 1)
			{
				sps = state->idx / (now - state->start);
				ft_printf("%d samples per second\n", (int) sps);
				state->last_update = now;
			}
			render_draw(state, colors, begin, end);
		}
	}
}

static void
	*thread_main(void *arg)
{
	t_rt_state		*state;
	t_thread_ctx	ctx;

	state = arg;
	context_create(&ctx, state);
	mutex_lock(&state->mtx);
	while (state->running)
	{
		thread_render(&ctx, state);
		if (RT_ONESHOT)
			state->running = 0;
		else if (state->running)
			cond_wait(&state->cnd, &state->mtx);
	}
	mutex_unlock(&state->mtx);
	context_destroy(&ctx);
	return (NULL);
}

void
	thread_reset(t_rt_state *state)
{
	size_t	i;

	state->idx = 0;
	state->version += 1;
	state->start = time_time();
	state->last_update = time_time();
	i = 0;
	while (i < state->size)
	{
		state->image[i] = vec(0, 0, 0, 0);
		i += 1;
	}
	cond_broadcast(&state->cnd);
}

void
	thread_start(t_rt_state *state)
{
	size_t	i;

	state->idx = 0;
	state->running = 1;
	state->version = 0;
	i = 0;
	while (i < RT_THREADS)
	{
		thread_create(&state->threads[i], thread_main, state);
		i += 1;
	}
}

void
	thread_stop(t_rt_state *state)
{
	size_t	i;

	mutex_lock(&state->mtx);
	state->running = 0;
	cond_broadcast(&state->cnd);
	mutex_unlock(&state->mtx);
	i = 0;
	while (i < RT_THREADS)
	{
		thread_join(&state->threads[i]);
		i += 1;
	}
}
