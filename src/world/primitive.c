#include "world.h"

static uint64_t
	world_primitive_size_adjust(uint64_t size)
{
	return ((size + RT_PRIMITIVE_ALIGN - 1) / RT_PRIMITIVE_ALIGN * RT_PRIMITIVE_ALIGN);
}

uint64_t
	world_primitive_size(uint8_t shape_type)
{
	if (shape_type == RT_SHAPE_SPHERE)
		return (world_primitive_size_adjust(sizeof(t_shape_sphere)));
	else
		return (world_primitive_size_adjust(sizeof(t_shape_triangle)));
}
