#include "gfx.h"

#include "util.h"

void
	img_create(t_img *img, int width, int height)	
{
	img->width = width;
	img->height = height;
	img->data = rt_malloc(sizeof(*img->data) * width * height);
}

void
	img_destroy(t_img *img)	
{
	rt_free(img->data);
}
