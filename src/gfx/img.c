#include "gfx.h"

#include "rt.h"
#include "util.h"
#include <libft.h>

void
	img_create(t_img *img, int width, int height)	
{
	img->width = width;
	img->height = height;
	img->data = rt_malloc(sizeof(*img->data) * width * height);
	mutex_init(&img->mtx);
}

void
	img_destroy(t_img *img)	
{
	rt_free(img->data);
	mutex_destroy(&img->mtx);
}

void
	img_clear(t_img *img, t_color color)
{
	size_t			index;
	size_t			size;
	t_color			*data;

	index = 0;
	size = img->width * img->height;
	mutex_lock(&img->mtx);
	data = img->data;
	while (index < size)
	{
		data[index] = color;
		index++;
	}
	mutex_unlock(&img->mtx);
}

void
	img_puti(t_img *img, int x, int y, t_color color)
{
	mutex_lock(&img->mtx);
	img->data[img->width * y + x] = color;
	mutex_unlock(&img->mtx);
}

void
	img_putf(t_img *img, int x, int y, t_vec color)
{
	(void) img;
	(void) x;
	(void) y;
	(void) color;
}

