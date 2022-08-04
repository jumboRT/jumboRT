#include "gfx.h"

#include "state.h"
#include <mlx.h>
#include <stddef.h>

void
	*mlx(void)
{
	static void *mlx = NULL;
	if (mlx == NULL)
		mlx = mlx_init();
	return (mlx);
}

int	win_create(t_win *win, int width, int height)
{
	int	ignore;

	win->handle = mlx_new_window(mlx(), width, height, "No Title");
	win->img = mlx_new_image(mlx(), width, height);
	win->data = mlx_get_data_addr(win->img, &win->bpp, &ignore, &ignore);
	return (0);
}

void win_start(int (*loop_proc)(void *ctx), void *ctx)
{
	mlx_loop_hook(mlx(), loop_proc, ctx);
	mlx_loop(mlx());
}

void win_key_hook(t_win *win, int (*callback)(int key, void *ctx), void *ctx)
{
	mlx_key_hook(win->handle, callback, ctx);
}

void win_event_hook(t_win *win, int event, int (*callback)(), void *ctx)
{
	if (event == RT_WIN_EVENT_KEY_DOWN)
		mlx_hook(win->handle, event, RT_WIN_EVENT_KEY_MASK, callback, ctx);
	else if (event == RT_WIN_EVENT_BUTTON_DOWN)
		mlx_hook(win->handle, event, RT_WIN_EVENT_BUTTON_MASK, callback, ctx);
	else
		mlx_hook(win->handle, event, 0, callback, ctx);
}

static int to_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return (
			((unsigned int) a << 24) |
			((unsigned int) r << 16) |
			((unsigned int) g <<  8) |
			((unsigned int) b));
}

static int color_at(const t_image *img, size_t index)
{
	t_pixel	pixel;
	t_vec	final_color;

	pixel = img->data[index];
	final_color = vec(1.0, 0.0, 1.0, 1.0);
	if (pixel.samples != 0)
		final_color = vec_scale(pixel.color, 1.0 / pixel.samples);
	final_color = vec(
			rt_sqrt(x(final_color)),
			rt_sqrt(y(final_color)),
			rt_sqrt(z(final_color)),
			0.0);
	final_color = vec_clamp(vec_scale(final_color, 255.0), 0, 255.0);
	return (to_color(
				(unsigned char) x(final_color),
				(unsigned char) y(final_color),
				(unsigned char) z(final_color),
				(unsigned char) 0));
}

void win_put(t_win *win, const t_image *img)
{
	size_t	index;
	size_t	size;
	int		bpp;
	char	*dest;

	index = 0;
	size = img->width * img->height;
	dest = win->data;
	bpp = win->bpp >> 3;
	while (index < size)
	{
		*(dest + (bpp * index)) = color_at(img, index) & 0xFF;
		*(dest + (bpp * index) + 1) = (color_at(img, index) >> 8) & 0xFF;
		*(dest + (bpp * index) + 2) = (color_at(img, index) >> 16) & 0xFF;
		*(dest + (bpp * index) + 3) = (color_at(img, index) >> 24) & 0xFF;
		index++;
	}
	mlx_put_image_to_window(mlx(), win->handle, win->img, 0, 0);
}
