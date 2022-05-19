#include "rt.h"

void
	render_range(t_rt_state *state, t_vec *dst,
		size_t begin, size_t end)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < end - begin)
	{
		j = state->order[begin + i];
		dst[i] = trace_pixel(state,
				j % state->img.width, j / state->img.width);
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
		j = state->order[begin + i];
		img_putf(&state->img,
			j % state->img.width, j / state->img.width, dst[i]);
		i += 1;
	}
}
