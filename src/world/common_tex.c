#include "world.h"
#include "mat.h"

GLOBAL unsigned char
	*get_tex_data(GLOBAL t_world *world, uint32_t index)
{
	return (world->texture_data + index);
}

const GLOBAL unsigned char
	*get_tex_data_const(const GLOBAL t_world *world, uint32_t index)
{
	return (world->texture_data + index);
}

const GLOBAL t_tex
	*get_tex_const(const GLOBAL t_world *world, uint32_t index)
{
	return (world->textures + index);
}
