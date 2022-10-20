#include "util.h"
#include "state.h"
#include "rtmath.h"
#include "net.h"
#include "ser.h"

#include <ft_printf.h>
#include <stdio.h>
#include <math.h>

static size_t
	rt_ppm_write_header(char *header, const t_image *image)
{
	return (ft_sprintf(header, "P6\n%d\n%d\n255\n",
			(int) image->width, (int) image->height));
}

static t_vec
	rt_pixel_color(const t_pixel *pixel)
{
	t_vec	res;

	res = vec(1.0, 0.0, 1.0, 0.0);
	if (pixel->samples != 0)
		res = vec_scale(pixel->color, 1.0 / pixel->samples);
	return (res);
}

static size_t
	rt_ppm_write_pixel(char *dest, const t_pixel *pixel)
{
	t_vec	final_color;

	final_color = rt_pixel_color(pixel);
	final_color = vec_gc(final_color);
	final_color = vec_clamp(vec_scale(final_color, 255.0), 0, 255.0);
	((unsigned char *) dest)[0] = (unsigned char) x(final_color);
	((unsigned char *) dest)[1] = (unsigned char) y(final_color);
	((unsigned char *) dest)[2] = (unsigned char) z(final_color);
	return (3);
}

static size_t
	rt_ppm_write_data(char *data, const t_image *image)
{
	size_t	x;
	size_t	y;
	size_t	offset;

	y = 0;
	offset = 0;
	while (y < image->height)
	{
		x = 0;
		while (x < image->width)
		{
			offset += rt_ppm_write_pixel(data + offset,
					&image->data[x + y * image->width]);
			x++;
		}
		y++;
	}
	return (offset);
}

size_t
	rt_image_to_ppm(char **dest, const t_image *image)
{
	size_t	size;
	int		offset;

	rt_assert(image->width > 0 && image->height > 0,
		"image width and height must be postive integers");
	size = (image->width * image->height) * 3 + 31;
	*dest = rt_malloc(size);
	offset = rt_ppm_write_header(*dest, image);
	offset += rt_ppm_write_data(*dest + offset, image);
	return (offset);
}
