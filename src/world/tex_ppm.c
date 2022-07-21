#include "tex.h"

#include "util.h"
#include <libft.h>
#include <ft_printf.h>

static unsigned long
	tex_read(unsigned char *file, size_t n)
{
	unsigned long	result;
	size_t			i;

	result = 0;
	i = 0;
	while (i < n)
	{
		result |= (unsigned long) file[i] << (i * 8);
		i += 1;
	}
	return (result);
}

void
	world_load_ppm(t_world *world, t_tex *tex, const unsigned char *file, size_t size)
{
	unsigned char	*file;
	unsigned char	*pixels;
	size_t			offset;
	size_t			bpp;
	size_t			row_size;
	size_t			x;
	size_t			y;

	offset = tex_read(file + 10, 4);
	tex->width = tex_read(file + 18, 4);
	tex->height = tex_read(file + 22, 4);
	bpp = tex_read(file + 28, 2);
	row_size = (bpp * tex->width + 31) / 32 * 4;
	if (size < offset + tex->height * row_size || tex->width == 0 || tex->height == 0)
		rt_assert(0, "bad image");
	tex->name = ft_strdup(name);
	tex->offset = world_alloc_tex_data(world, tex->width * 3 * tex->height);
	pixels = world_get_tex_data(world, offset);
	y = 0;
	while (y < tex->height)
	{
		x = 0;
		while (x < tex->width)
		{
			pixels[y * tex->width + x + 0] = (unsigned char) tex_read(file + offset + y * row_size + x * bpp / 8 + 0, 1);
			pixels[y * tex->width + x + 1] = (unsigned char) tex_read(file + offset + y * row_size + x * bpp / 8 + 1, 1);
			pixels[y * tex->width + x + 2] = (unsigned char) tex_read(file + offset + y * row_size + x * bpp / 8 + 2, 1);
			x += 3;
		}
		y += 1;
	}
}

void
	tex_destroy(t_tex *tex)
{
	rt_free(tex->pixels);
}
