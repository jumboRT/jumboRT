#ifndef ACCEL_H
# define ACCEL_H

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

typedef struct s_world				t_world;
typedef struct s_primitive			t_primitive;

typedef struct s_accel_node			t_accel_node;
typedef struct s_rope_data			t_rope_data;
typedef struct s_leaf_data			t_leaf_data;
typedef struct s_prim_info			t_prim_info;
typedef struct s_tree_edges			t_tree_edges;
typedef struct s_tree_info			t_tree_info;
typedef struct s_node_info			t_node_info;
typedef struct s_split				t_split;
typedef struct s_edge				t_edge;

struct s_rope_data {
	float		bounds[6];
	uint32_t	ropes[6];
};

struct s_leaf_data {
	t_rope_data		rope_data;
};

# if defined ACCEL_USE_ROPES

struct s_accel_node {
	union {
		float		split;
		uint32_t	inline_primitives[ACCEL_INLINE_PRIMS];
		uint32_t	primitive_offset;
	}	u_a;
	union {
		uint32_t	flags;
		uint32_t	nprims;
		uint32_t	above_child;
	}	u_b;
	uint32_t	leaf_data_index;
};

# else

struct s_accel_node {
	union {
		float		split;
		uint32_t	inline_primitives[ACCEL_INLINE_PRIMS];
		uint32_t	primitive_offset;
	}	u_a;
	union {
		uint32_t	flags;
		uint32_t	nprims;
		uint32_t	above_child;
	}	u_b;
};
# endif

struct s_node_info {
	t_tree_info		*tree;
	uint32_t		offset;
	uint32_t		depth;
	t_tree_edges	*edges;
	t_bounds		bounds;
};

struct s_split {
	float		offset;
	float		cost;
	uint8_t		axis;
};

float				split_pos(t_accel_node node);
uint32_t			nprims(t_accel_node node);
uint32_t			split_axis(t_accel_node node);
uint32_t			is_leaf(t_accel_node node);
uint32_t			above_child(t_accel_node node);

#endif
