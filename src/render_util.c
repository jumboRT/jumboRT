#include "rt.h"

#include <math.h>

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
		dst[i].v[X] = fmin(dst[i].v[X], 1.0);
		dst[i].v[Y] = fmin(dst[i].v[Y], 1.0);
		dst[i].v[Z] = fmin(dst[i].v[Z], 1.0);
		dst[i].v[W] = 1.0;
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
		i += 1;
	}
}
