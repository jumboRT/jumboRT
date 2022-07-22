#include "world_impl.h"

#include <limits.h>

void
	material_init(t_material *mat)
{
	mat->emission = vec(0, 0, 0, 0);
	mat->albedo = vec(0, 0, 0, 1);
	mat->refractive = 0;
	mat->refractive_index = 1;
	mat->reflective = 0;
	mat->density = 0;
	mat->brightness = 1.0;
	mat->has_texture = 0;
	mat->id = UINT_MAX;
}

void
	texture_init(t_tex *tex)
{
	tex->width = ULONG_MAX;
	tex->height = ULONG_MAX;
	tex->offset = UINT_MAX;
}
