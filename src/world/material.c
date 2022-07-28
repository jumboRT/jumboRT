#include "world_impl.h"

#include <limits.h>

void
	material_init(t_material *mat, const t_world *world)
{
	mat->flags = 0;
	mat->bxdf_begin = world->bxdfs_size / RT_BXDF_ALIGN;
	mat->bxdf_end = world->bxdfs_size / RT_BXDF_ALIGN;
	mat->emission = 0;
	mat->brightness = 0.0;
}

void
	texture_init(t_tex *tex)
{
	tex->type = RT_TEX_COLOR;
	tex->a.color = vec(0, 0, 0, 0);
}
