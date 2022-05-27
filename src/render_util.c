#include "rt.h"

void
	render_range(t_thread_ctx *ctx, t_rt_state *state, t_vec *dst,
		size_t begin, size_t end)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < end - begin)
	{
		j = state->order[(begin + i) % state->size];
		dst[i] = trace(ctx, state, j % state->width, j / state->width);
		i += 1;
	}
}

void
	render_draw(t_rt_state *state, t_vec *dst,
		size_t begin, size_t end)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < end - begin)
	{
		j = state->order[(begin + i) % state->size];
		state->image[j] = vec_add(state->image[j], dst[i]);
		state->samples[j] += 1;
		i += 1;
	}
}
