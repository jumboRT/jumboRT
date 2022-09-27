#ifndef ACCEL_IMPL_H
# define ACCEL_IMPL_H

#include "accel.h"
#include <stddef.h>

struct s_edge {
	float		offset;
	uint32_t	index : 31;
	uint32_t	type : 1;
};

struct s_tree_edges {
	t_edge	*edges[3];
	size_t	count;
};

struct s_tree_info {
	t_world			*world;
	t_prim_info		*prims;
	size_t			prims_count;
	t_tree_edges	*edges;
};

void				world_info_create(t_tree_info *tree, t_node_info *node,
						t_world *world);
void				world_info_init(t_tree_info *tree, t_node_info *node,
						t_world *world);
void				world_info_destroy(t_tree_info *tree, t_node_info *node);


int					world_axis_side(const t_tree_info *tree,
						const t_split *split, uint32_t index);

const t_primitive	*get_primitive(const t_tree_info *info, uint64_t index);

void				interior_node_init(t_node_info *parent_info,
						const t_node_info *above_info, const t_split *split);
void				leaf_node_init(t_node_info *node_info);
float				get_split_cost(const t_bounds bounds, const t_split *split,
						const uint32_t primitive_counts[2]);

uint32_t			world_max_depth(uint64_t prims_count);
uint32_t			new_node(t_world *world);
t_bounds			get_bounds(const t_world *world, const t_primitive *primitive);

#endif
