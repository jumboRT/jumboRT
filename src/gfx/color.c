#include "gfx.h"

#include <mlx.h>

static int
	to_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return (
		((unsigned int) a << 24)
		| ((unsigned int) r << 16)
		| ((unsigned int) g << 8)
		| ((unsigned int) b));
}

static int
	color_at(const t_image *img, size_t index)
{
	t_pixel	pixel;
	t_vec	final_color;

	pixel = img->data[index];
	final_color = vec(1.0, 0.0, 1.0, 1.0);
	if (pixel.samples != 0)
		final_color = vec_scale(pixel.color, 1.0 / pixel.samples);
	final_color = vec_gc(final_color);
	final_color = vec_clamp(vec_scale(final_color, 255.0), 0, 255.0);
	return (to_color(
			(unsigned char) x(final_color),
			(unsigned char) y(final_color),
			(unsigned char) z(final_color),
			(unsigned char) 0));
}

void
	win_put(t_win *win, const t_image *img)
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
