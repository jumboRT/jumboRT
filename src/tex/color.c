#include "tex.h"

#include "util.h"

const t_texture_vt
	*color_texture_vt(void)
{
	static const t_texture_vt	vt = {
		color_texture_sample,
		color_texture_destroy
	};

	return (&vt);
}

t_vec
	color_texture_sample(t_texture *tex, t_vec uv)
{
	t_color_texture	*color;

	(void) uv;
	color = (t_color_texture *) tex;
	return (color->color);
}

void
	color_texture_destroy(t_texture *tex)
{
	rt_free(tex);
}

