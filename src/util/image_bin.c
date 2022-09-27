#include "util.h"
#include "state.h"
#include "rtmath.h"
#include "net.h"
#include "ser.h"

#include <ft_printf.h>
#include <stdio.h>

static size_t 
	rt_ppm_write_header(char *header, const t_image *image)
{
	return (ft_sprintf(header,"P6\n%d\n%d\n255\n", (int) image->width, (int) image->height));
}

static size_t
	rt_pfm_write_header(char *header, const t_image *image, float max)
{
	/* TODO: replace with own? snprintf for float or smth */
	return (sprintf(header, "PF\n%d\x20%d\n-%f\n", (int) image->width,
				(int) image->height, max));
}

static t_vec rt_pixel_color(const t_pixel *pixel)
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
	rt_pfm_write_color(char *dst, t_vec color)
{
	char *cpy;

	cpy = rt_packvec(dst, color);
	return (cpy - dst);
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
			offset += rt_ppm_write_pixel(data + offset, &image->data[x + y * image->width]);
			x++;
		}
		y++;
	}
	return (offset);
}

static size_t
	rt_pfm_write_data(char *data, const t_image *image, float *max)
{
	size_t			x;
	size_t			y;
	size_t			offset;
	t_vec			color;
	const t_pixel	*pixel;

	y = image->height;
	offset = 0;
	*max = RT_TINY_VAL;
	while (y--)
	{
		x = 0;
		while (x < image->width)
		{
			pixel = &image->data[x + y * image->width];
			color = rt_pixel_color(pixel);
			if (vec_maxv(color) > *max)
				*max = vec_maxv(color);
			offset += rt_pfm_write_color(data + offset, color);
			x++;
		}
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

size_t
	rt_image_to_pfm(char **dest, const t_image *image)
{
	size_t	size;
	size_t	data_size;
	int		offset;
	float	max;
	char	*tmp;

	rt_assert(image->width > 0 && image->height > 0,
			"image width and height must be postive integers");
	size = (image->width * image->height) * 12 + 79; 
	*dest = rt_malloc(size);
	tmp = rt_malloc(image->width * image->height * 12);
	data_size = rt_pfm_write_data(tmp, image, &max);
	offset = rt_pfm_write_header(*dest, image, max);
	rt_memcpy(*dest + offset, tmp, image->width * image->height * 12);
	rt_free(tmp);
	return (offset + data_size);
}

size_t
	rt_write_pxm(const char *path, const t_image *image,
			size_t (*proc)(char**, const t_image*))
{
	char	*error;
	size_t	size;
	char	*ppm;

	size = proc(&ppm, image);
	if (rt_writefile(path, &error, ppm, size) < 0)
		rt_assert(0, error);
	rt_free(ppm);
	return (size);
}

size_t
	rt_write_ppm(const char *path, const t_image *image)
{
	return (rt_write_pxm(path, image, rt_image_to_ppm));
}

size_t
	rt_write_pfm(const char *path, const t_image *image)
{
	return (rt_write_pxm(path, image, rt_image_to_pfm));
}
