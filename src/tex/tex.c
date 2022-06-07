#include "tex.h"

#include "util.h"
#include "libft.h"
#include "ft_printf.h"

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

static t_tex
	*tex_load_file(const char *name, char **error)
{
	unsigned char	*file;
	size_t			size;
	t_tex			tex;
	size_t			offset;
	size_t			bpp;
	size_t			row_size;
	size_t			x;
	size_t			y;

	file = (unsigned char *) rt_readfile(name, error, &size);
	if (file == NULL || size < 26)
	{
		rt_free(file);
		return (NULL);
	}
	offset = tex_read(file + 10, 4);
	tex.width = tex_read(file + 18, 4);
	tex.height = tex_read(file + 22, 4);
	bpp = tex_read(file + 28, 2);
	row_size = (bpp * tex.width + 31) / 32 * 4;
	if (size < offset + tex.height * row_size || tex.width == 0 || tex.height == 0)
	{
		rt_free(file);
		return (NULL);
	}
	tex.name = ft_strdup(name);
	tex.pixels = rt_malloc(sizeof(*tex.pixels) * tex.width * tex.height);
	y = 0;
	while (y < tex.height)
	{
		x = 0;
		while (x < tex.width)
		{
			tex.pixels[y * tex.width + x].v[Z] = tex_read(file + offset + y * row_size + x * bpp / 8 + 0, 1) / 255.0;
			tex.pixels[y * tex.width + x].v[Y] = tex_read(file + offset + y * row_size + x * bpp / 8 + 1, 1) / 255.0;
			tex.pixels[y * tex.width + x].v[X] = tex_read(file + offset + y * row_size + x * bpp / 8 + 2, 1) / 255.0;
			tex.pixels[y * tex.width + x].v[W] = 0;
			x += 1;
		}
		y += 1;
	}
	rt_free(file);
	return (rt_memdup(&tex, sizeof(tex)));
}

void
	tex_create(t_tex_list *texs)
{
	texs->list = NULL;
	texs->count = 0;
}

void
	tex_destroy(t_tex_list *texs)
{
	size_t	i;

	i = 0;
	while (i < texs->count)
	{
		rt_free(texs->list[i]->pixels);
		rt_free(texs->list[i]->name);
		rt_free(texs->list[i]);
		i += 1;
	}
	rt_free(texs->list);
}

t_tex
	*tex_load(t_tex_list *texs, const char *name, char **error)
{
	size_t	i;
	t_tex	*tex;

	i = 0;
	while (i < texs->count)
	{
		if (ft_strcmp(texs->list[i]->name, name) == 0)
			return (texs->list[i]);
		i += 1;
	}
	tex = tex_load_file(name, error);
	if (tex == NULL)
	{
		if (*error == NULL)
			ft_asprintf(error, "Bad image");
		return (NULL);
	}
	texs->list = rt_realloc(texs->list,
		sizeof(*texs->list) * (texs->count + 0),
		sizeof(*texs->list) * (texs->count + 1));
	texs->list[texs->count] = tex;
	texs->count += 1;
	return (tex);
}

t_vec
	tex_sample(t_tex *tex, t_vec uv)
{
	size_t	x;
	size_t	y;

	x = (size_t)(uv.v[U] * tex->width);
	y = (size_t)(uv.v[V] * tex->height);
	if (x < 0)
		x = 0;
	if (x >= tex->width)
		x = tex->width - 1;
	if (y < 0)
		y = 0;
	if (y >= tex->height)
		y = tex->height - 1;
	y = tex->height - 1 - y;
	return (tex->pixels[y * tex->width + x]);
}

