#include "world_impl.h"

#include "util.h"
#include <libft.h>
#include <ft_printf.h>

static unsigned long
	tex_read(const unsigned char *file, size_t n)
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
	unsigned char	*pixels;
	size_t			offset;
	size_t			bpp;
	size_t			row_size;
	size_t			x;
	size_t			y;

	offset = tex_read(file + 10, 4);
	tex->a.tex.width = tex_read(file + 18, 4);
	tex->a.tex.height = tex_read(file + 22, 4);
	bpp = tex_read(file + 28, 2);
	rt_assert(bpp == 32, "miniRT only supports 32-bit bmp");
	row_size = (bpp * tex->a.tex.width + 31) / 32 * 4;
	if (size < offset + tex->a.tex.height * row_size || tex->a.tex.width == 0 || tex->a.tex.height == 0)
		rt_assert(0, "bad image");
	tex->a.tex.offset = world_alloc_tex_data(world, tex->a.tex.width * 4 * tex->a.tex.height);
	pixels = get_tex_data(world, tex->a.tex.offset);
	y = 0;
	while (y < tex->a.tex.height)
	{
		x = 0;
		while (x < tex->a.tex.width)
		{
			pixels[(y * tex->a.tex.width + x) * 4 + 2] = (unsigned char) tex_read(file + offset + y * row_size + x * bpp / 8 + 0, 1);
			pixels[(y * tex->a.tex.width + x) * 4 + 1] = (unsigned char) tex_read(file + offset + y * row_size + x * bpp / 8 + 1, 1);
			pixels[(y * tex->a.tex.width + x) * 4 + 0] = (unsigned char) tex_read(file + offset + y * row_size + x * bpp / 8 + 2, 1);
			pixels[(y * tex->a.tex.width + x) * 4 + 3] = (unsigned char) tex_read(file + offset + y * row_size + x * bpp / 8 + 3, 1);
			x += 1;
		}
		y += 1;
	}
}
