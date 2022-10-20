#include "world.h"
#include "mat.h"

t_vec
	get_vertex(const t_world *world, uint32_t index)
{
	return (world->vertices[index].pos);
}

const GLOBAL t_bxdf
	*get_bxdf_const(const GLOBAL t_world *world, uint32_t index)
{
	return (&world->bxdfs[index].base);
}

GLOBAL t_bxdf
	*get_bxdf(GLOBAL t_world *world, uint32_t index)
{
	return (&world->bxdfs[index].base);
}
