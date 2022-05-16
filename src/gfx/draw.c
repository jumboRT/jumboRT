#include "gfx.h"

#include <libft.h>
#include <mlx.h>

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
		*(int *)(dest + (bpp * index)) = img->data[index];
		index++;
	}
	mutex_unlock(&img->mtx);
	mlx_put_image_to_window(win->mlx, win->handle, win->img, 0, 0);
}
