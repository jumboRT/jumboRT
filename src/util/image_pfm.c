/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   image_pfm.c                                    #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:16 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:16 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "state.h"
#include "rtmath.h"
#include "net.h"
#include "ser.h"

#include <ft_printf.h>
#include <stdio.h>
#include <math.h>

/* poor man's float conversion */
static size_t
	rt_pfm_write_header(char *header, const t_image *image, float max)
{
	unsigned int	integer;
	unsigned int	fraction;

	integer = (unsigned) floor(max);
	fraction = (unsigned)(fmod(max, 1.0f) * 1000);
	return (ft_sprintf(header, "PF\n%d\x20%d\n-%u.%03u\n", (int) image->width,
			(int) image->height, integer, fraction));
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
	rt_pfm_write_color(char *dst, t_vec color)
{
	char	*cpy;

	cpy = rt_packvec(dst, color);
	return (cpy - dst);
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
			if (vec_xyz_maxv(color) > *max)
				*max = vec_xyz_maxv(color);
			offset += rt_pfm_write_color(data + offset, color);
			x++;
		}
	}
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
