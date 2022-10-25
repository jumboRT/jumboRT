/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   accel_impl.h                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:48 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 13:03:50 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACCEL_IMPL_H
# define ACCEL_IMPL_H

# include "typedefs.h"
# include "accel.h"
# include <stddef.h>

struct s_edge {
	float		offset;
	t_uint32	index : 31;
	t_uint32	type : 1;
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

struct s_accel_init_ctx {
	t_tree_info		tree;
	t_node_info		node;
	t_accel_node	root;
};

void				world_best_split_axis(t_node_info *node, t_split *best,
						t_uint8 axis);
int					world_accel_load(t_world *world, const char *file,
						t_uint64 hash);
void				world_accel_save(t_world *world, const char *file,
						t_uint64 hash);
void				world_info_create(t_tree_info *tree, t_node_info *node,
						t_world *world);
void				world_info_init(t_tree_info *tree, t_node_info *node,
						t_world *world);
void				world_info_destroy(t_tree_info *tree, t_node_info *node);

int					world_axis_side(const t_tree_info *tree,
						const t_split *split, t_uint32 index);

const t_primitive	*get_primitive(const t_tree_info *info, size_t index);

void				interior_node_init(t_node_info *parent_info,
						const t_node_info *above_info, const t_split *split);
void				leaf_node_init(t_node_info *node_info);
float				get_split_cost(const t_bounds bounds, const t_split *split,
						const t_uint32 primitive_counts[2]);

t_uint32			world_max_depth(size_t prims_count);
t_uint32			new_node(t_world *world);
t_bounds			get_bounds(const t_world *world,
						const t_primitive *primitive);

#endif
