#include "tex.h"

#include "util.h"

const t_texture_vt
	*image_texture_vt(void)
{
	static const t_texture_vt	vt = {
		image_texture_sample,
		image_texture_destroy
	};

	return (&vt);
}

t_vec
	image_texture_sample(t_texture *tex, t_vec uv)
{
	t_image_texture	*image;

	image = (t_image_texture *) tex;
	return (tex_sample(image->tex, uv));
}

void
	image_texture_destroy(t_texture *tex)
{
	rt_free(tex);
}

