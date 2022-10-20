#include "world.h"
#include "shape.h"
#include "mat.h"

uint32_t
	prim_mat(const GLOBAL t_primitive *prim)
{
	return (prim->data >> 8);
}

const GLOBAL t_primitive
	*get_prim_const(const GLOBAL t_world *world, uint32_t index)
{
	return ((const GLOBAL t_primitive *)((const GLOBAL char *)world->primitives
		+ (uint64_t)index * RT_PRIMITIVE_ALIGN));
}

GLOBAL t_primitive
	*get_prim(GLOBAL t_world *world, uint32_t index)
{
	return ((GLOBAL t_primitive *)((GLOBAL char *)world->primitives
		+ (uint64_t)index * RT_PRIMITIVE_ALIGN));
}

const GLOBAL t_material
	*get_mat_const(const GLOBAL t_world *world, uint32_t index)
{
	return (&world->materials[index]);
}

GLOBAL t_material
	*get_mat(GLOBAL t_world *world, uint32_t index)
{
	return (&world->materials[index]);
}
