#include "gfx.h"
#include "rt.h"

#include <libft.h>
#include <mlx.h>

/* TODO: this function is no longer used */
/* TODO: t_img is no longer used either i think */
void
	win_put(t_win *win, t_img *img)
{
	size_t	index;
	size_t	size;
	int		bpp;
	char	*dest;

	index = 0;
	size = img->width * img->height;
	dest = win->data;
	bpp = win->bpp >> 3;
	mutex_lock(&img->mtx);
	while (index < size)
	{
		*(dest + (bpp * index)) = img->data[index] & 0xFF;
		*(dest + (bpp * index) + 1) = (img->data[index] >> 8) & 0xFF;
		*(dest + (bpp * index) + 2) = (img->data[index] >> 16) & 0xFF;
		*(dest + (bpp * index) + 3) = (img->data[index] >> 24) & 0xFF;
		index++;
	}
	mutex_unlock(&img->mtx);
	mlx_put_image_to_window(win->mlx, win->handle, win->img, 0, 0);
}

void
	win_put_state(t_rt_state *state)
{
	size_t	i;
	int		bpp;
	char	*dst;
	t_vec	col;
	size_t	samples;
	FLOAT	done;

	i = 0;
	dst = state->win.data;
	bpp = state->win.bpp >> 3;
	done = (FLOAT) state->idx / state->end;
	mutex_lock(&state->mtx);
	while (i < state->size)
	{
		samples = state->samples[i];
		if (samples == 0)
			samples = 1;
		col = vec_scale(state->image[i], 255.0 / samples);
		dst[bpp * i + 3] = (t_color)(col.v[W]) & 0xFF;
		dst[bpp * i + 2] = (t_color)(col.v[X]) & 0xFF;
		dst[bpp * i + 1] = (t_color)(col.v[Y]) & 0xFF;
		dst[bpp * i + 0] = (t_color)(col.v[Z]) & 0xFF;
		i += 1;
	}
	if (state->idx != state->end)
	{
		i = 0;
		while (i < state->width)
		{
			dst[bpp * i + 3] = 0x00;
			if ((FLOAT) i / state->width < done)
			{
				dst[bpp * i + 2] = 0xFF;
				dst[bpp * i + 1] = 0xFF;
				dst[bpp * i + 0] = 0xFF;
			}
			else
			{
				dst[bpp * i + 2] = 0x00;
				dst[bpp * i + 1] = 0x00;
				dst[bpp * i + 0] = 0x00;
			}
			i += 1;
		}
	}
	mutex_unlock(&state->mtx);
	mlx_put_image_to_window(state->win.mlx, state->win.handle, state->win.img, 0, 0);
}
