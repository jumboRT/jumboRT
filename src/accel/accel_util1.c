#include "accel.h"
#include "accel_impl.h"
#include "util.h"
#include "world.h"
#include "world_impl.h"
#include <math.h>

const t_primitive
	*get_primitive(const t_tree_info *info, size_t index) {
	return (get_prim_const(info->world, info->prims[index].index));
}

int
	world_axis_side(const t_tree_info *tree, const t_split *split,
			uint32_t index) {
	t_bounds	prim_bounds;
	float		min_offset;
	float		max_offset;
	float		axis_offset;

	prim_bounds = tree->prims[index].bounds;
	max_offset = xyz(prim_bounds.max, split->axis);
	min_offset = xyz(prim_bounds.min, split->axis);
	axis_offset = split->offset;
	if (max_offset >= axis_offset && min_offset >= axis_offset)
		return (1);
	else if (max_offset <= axis_offset && min_offset <= axis_offset)
		return (-1);
	return (0);
}
