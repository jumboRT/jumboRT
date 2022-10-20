#include "accel_impl.h"

static inline float
	calculate_cost(float total_sa, const float sub_sa[2],
		const uint32_t primitive_count[2])
{
	float	inv_total_sa;
	float	below_chance;
	float	above_chance;
	float	cost;

	inv_total_sa = 1.0 / total_sa;
	below_chance = sub_sa[ACCEL_BELOW] * inv_total_sa;
	above_chance = sub_sa[ACCEL_ABOVE] * inv_total_sa;
	cost = (RT_TRAVERSAL_COST + RT_INTERSECT_COST
			* (1.0 - (RT_EMPTY_BONUS * (primitive_count[ACCEL_BELOW] == 0
						|| primitive_count[ACCEL_ABOVE] == 0)))
			* (below_chance * primitive_count[ACCEL_BELOW]
				+ above_chance * primitive_count[ACCEL_ABOVE]));
	return (cost);
}

static void
	get_surface_areas(const t_bounds bounds, const float offset,
		const int axis, float out[2])
{
	t_vec	diagonal;
	int		other_axis0;
	int		other_axis1;

	diagonal = vec_sub(bounds.max, bounds.min);
	other_axis0 = (axis + 1) % 3;
	other_axis1 = (axis + 2) % 3;
	out[ACCEL_BELOW] = 2.0
		* (xyz(diagonal, other_axis0) * xyz(diagonal, other_axis1)
			+ (offset - xyz(bounds.min, axis))
			* (xyz(diagonal, other_axis0) + xyz(diagonal, other_axis1)));
	out[ACCEL_ABOVE] = 2.0
		* (xyz(diagonal, other_axis0) * xyz(diagonal, other_axis1)
			+ (xyz(bounds.max, axis) - offset)
			* (xyz(diagonal, other_axis0) + xyz(diagonal, other_axis1)));
}

float
	get_split_cost(const t_bounds bounds, const t_split *split,
		const uint32_t primitive_counts[2])
{
	float		surface_areas[2];

	get_surface_areas(bounds, split->offset, split->axis, surface_areas);
	return (calculate_cost(bounds_surf(bounds),
			surface_areas, primitive_counts));
}

uint32_t
	world_max_depth(size_t prims_count)
{
	if (prims_count <= 1)
		return (1);
	return (8.0 + 1.3 * log2((float) prims_count));
}

uint32_t
	new_node(t_world *world)
{
	t_accel_node	new_node;

	return (world_add_accel_node(world, &new_node));
}
