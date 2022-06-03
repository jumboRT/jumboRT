#include "tree.h"

#include <math.h>

static void 
	gen_bounding_box(t_vec points[2], t_entity **list, size_t count)
{
	size_t	index;

	points[0] = vec(HUGE_VAL, HUGE_VAL, HUGE_VAL, 0);
	points[1] = vec(-HUGE_VAL, -HUGE_VAL, -HUGE_VAL, 0);
	index = 0;
	while (index < count)
	{
		points[0] = vec_min(list[index]->vt->get_pos(list[index]), points[0]);
		points[1] = vec_max(list[index]->vt->get_pos(list[index]), points[1]);
		index++;
	}
}

/* Count may not be less than 2 */
t_tree_plane
	*tree_gen_planes(t_entity **list, size_t count, size_t *out_count)
{
	size_t			dir_index;
	size_t			step_index;
	size_t			index;
	t_vec			bounding_box[2];
	t_vec			mid;
	t_tree_plane	*planes;
	t_vec			normal;
	static t_seed	seed = 142531425314253;

	gen_bounding_box(bounding_box, list, count);
	mid = vec_scale(vec_add(bounding_box[0], bounding_box[1]), 0.5);
	planes = rt_malloc(sizeof(*planes) * RT_TREE_STEPS * RT_TREE_DIRECTIONS);
	dir_index = 0;
	index = 0;
	while (dir_index < RT_TREE_DIRECTIONS)
	{
		normal = rt_random_svec(&seed);
		step_index = 0;
		while (step_index < RT_TREE_STEPS)
		{
			planes[index].dir = normal;
			planes[index].pos = vec_add(mid, vec_scale(normal, (FLOAT) step_index / RT_TREE_STEPS));
			index++;
			step_index++;
		}
		dir_index++;
	}
	*out_count = RT_TREE_STEPS * RT_TREE_DIRECTIONS;
	return (planes);
}

