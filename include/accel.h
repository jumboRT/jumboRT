/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   accel.h                                        #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:42 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/27 13:10:10 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACCEL_H
# define ACCEL_H

# include "typedefs.h"
# include "types.h"
# include "aabb.h"

# define ACCEL_BELOW 0
# define ACCEL_ABOVE 1
# define EDGE_START 0
# define EDGE_END 1
# define AXIS_X 0
# define AXIS_Y 1
# define AXIS_Z 2
# define AXIS_NONE 3

# define RT_INTERSECT_COST 80
# define RT_TRAVERSAL_COST 80
# define RT_EMPTY_BONUS (float) 0.3
# define RT_MAX_PRIMITIVES 3

# ifndef ACCEL_USE_TREE
#  define ACCEL_USE_TREE 1
# endif
# ifndef ACCEL_USE_ROPES
#  define ACCEL_USE_ROPES 1
# endif
# ifndef ACCEL_INLINE_PRIMS
#  define ACCEL_INLINE_PRIMS 1
# endif

struct s_rope_data {
	float		bounds[6];
	t_uint32	ropes[6];
};

struct s_leaf_data {
	t_rope_data		rope_data;
};

# if defined ACCEL_USE_ROPES

struct s_accel_node {
	union {
		float		split;
		t_uint32	inline_primitives[ACCEL_INLINE_PRIMS];
		t_uint32	primitive_offset;
	}	u_a;
	union {
		t_uint32	flags;
		t_uint32	nprims;
		t_uint32	above_child;
	}	u_b;
	t_uint32	leaf_data_index;
};

# else

struct s_accel_node {
	union {
		float		split;
		t_uint32	inline_primitives[ACCEL_INLINE_PRIMS];
		t_uint32	primitive_offset;
	}	u_a;
	union {
		t_uint32	flags;
		t_uint32	nprims;
		t_uint32	above_child;
	}	u_b;
};
# endif

struct s_node_info {
	t_tree_info		*tree;
	t_uint32		offset;
	t_uint32		depth;
	t_tree_edges	*edges;
	t_bounds		bounds;
};

struct s_split {
	float		offset;
	float		cost;
	t_uint8		axis;
};

float				split_pos(t_accel_node node);
t_uint32			nprims(t_accel_node node);
t_uint32			split_axis(t_accel_node node);
t_uint32			is_leaf(t_accel_node node);
t_uint32			above_child(t_accel_node node);

#endif
