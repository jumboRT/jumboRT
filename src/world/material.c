#include "world_impl.h"

#include <limits.h>

void
	material_init(t_material *mat, const t_world *world)
{
	mat->flags = 0;
	mat->surface.begin = world->bxdfs_count;
	mat->surface.end = world->bxdfs_count;
	mat->surface.weight = 0.0;
	mat->volume.begin = world->bxdfs_count;
	mat->volume.end = world->bxdfs_count;
	mat->volume.weight = 0.0;
	mat->brightness = 0.0;
}

void
	texture_init(t_tex *tex)
{
	tex->type = RT_TEX_COLOR;
	tex->a.color = vec(0, 0, 0, 0);
}
