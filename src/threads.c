#include "rt.h"

static void
	thread_render(t_rt_state *state)
{
	size_t	begin;
	size_t	end;
	long	version;
	t_vec	colors[RT_RENDER_CHUNK_SIZE];

	while (state->idx < state->size)
	{
		begin = state->idx;
		end = begin + RT_RENDER_CHUNK_SIZE;
		if (end >= state->size)
			end = state->size;
		state->idx = end;
		version = state->version;
		mutex_unlock(&state->mtx);
		render_range(state, colors, begin, end);
		mutex_lock(&state->mtx);
		if (version >= state->version)
			render_draw(state, colors, begin, end);
	}
}

static void
	*thread_main(void *arg)
{
	t_rt_state	*state;

	state = arg;
	mutex_lock(&state->mtx);
	while (state->running)
	{
		thread_render(state);
		if (RT_ONESHOT)
			state->running = 0;
		else if (state->running)
			cond_wait(&state->cnd, &state->mtx);
	}
	mutex_unlock(&state->mtx);
	return (NULL);
}

void
	thread_reset(t_rt_state *state)
{
	size_t	i;

	mutex_lock(&state->mtx);
	state->idx = 0;
	state->version += 1;
	i = 0;
	while (i < state->size)
	{
		img_putf(&state->img,
			i % state->img.width, i / state->img.width, vec(0, 0, 0, 0));
		i += 1;
	}
	cond_broadcast(&state->cnd);
	mutex_unlock(&state->mtx);
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
