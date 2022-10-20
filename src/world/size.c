#include "world.h"

#include "shape.h"

static uint64_t
	world_primitive_size_adjust(uint64_t size)
{
	return ((size + RT_PRIMITIVE_ALIGN - 1) / RT_PRIMITIVE_ALIGN
		* RT_PRIMITIVE_ALIGN);
}

uint64_t
	world_primitive_size(uint8_t shape_type)
{
	if (shape_type == RT_SHAPE_SPHERE)
		return (world_primitive_size_adjust(sizeof(t_shape_sphere)));
	else if (shape_type == RT_SHAPE_PLANE)
		return (world_primitive_size_adjust(sizeof(t_shape_plane)));
	else if (shape_type == RT_SHAPE_CYLINDER)
		return (world_primitive_size_adjust(sizeof(t_shape_cylinder)));
	else if (shape_type == RT_SHAPE_CONE)
		return (world_primitive_size_adjust(sizeof(t_shape_cone)));
	else if (shape_type == RT_SHAPE_TRIANGLE)
		return (world_primitive_size_adjust(sizeof(t_shape_triangle)));
	else if (shape_type == RT_SHAPE_PARABOLOID)
		return (world_primitive_size_adjust(sizeof(t_shape_paraboloid)));
	else if (shape_type == RT_SHAPE_HYPERBOLOID)
		return (world_primitive_size_adjust(sizeof(t_shape_hyperboloid)));
	else if (shape_type == RT_SHAPE_POINT)
		return (world_primitive_size_adjust(sizeof(t_shape_point)));
	return (RT_PRIMITIVE_ALIGN);
}
