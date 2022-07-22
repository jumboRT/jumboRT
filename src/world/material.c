#include "world_impl.h"

#include <limits.h>

void
	material_init(t_material *mat)
{
	mat->emission = vec(0, 0, 0);
	mat->albedo = vec(0, 0, 0);
	mat->refractive = 0;
	mat->refractive_index = 1;
	mat->reflective = 0;
	mat->density = 0;
	mat->has_texture = 0;
	mat->id = UINT_MAX;
}
