#ifndef WORLD_IMPL_H
# define WORLD_IMPL_H

# include "world.h"
# include "state.h"
# include "vector.h"
# include "aabb.h"
# include "pool.h"

# include <stddef.h>

/* TODO set these values to something good */
# define RT_INTERSECT_COST 80
# define RT_TRAVERSAL_COST 80
# define RT_EMPTY_BONUS (FLOAT) 0.3
# define RT_MAX_PRIMITIVES 20

# define ACCEL_BELOW 0
# define ACCEL_ABOVE 1
# define EDGE_START 0
# define EDGE_END 1
# define AXIS_X 0
# define AXIS_Y 1
# define AXIS_Z 2
# define AXIS_NONE 3

typedef struct s_prim_info	t_prim_info;
typedef struct s_tree_edges	t_tree_edges;
typedef struct s_tree_info	t_tree_info;
typedef struct s_node_info	t_node_info;
typedef struct s_split		t_split;
typedef struct s_edge		t_edge;

struct s_prim_info {
	uint32_t	index;
	t_bounds	bounds;
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

struct s_node_info {
	t_tree_info		*tree;
	uint32_t		offset;
	uint32_t		depth;
	t_tree_edges	*edges;
	t_bounds		bounds;
};

struct s_split {
	FLOAT		offset;
	FLOAT		cost;
	uint8_t		axis;
};

struct s_edge {
	FLOAT		offset;
	uint32_t	index : 31;
	uint32_t	type : 1;
};

void		world_create(t_world *world);
void		world_destroy(t_world *world);

uint32_t	world_add_material(t_world *world, void *material, size_t size);
uint32_t	world_add_primitive(t_world *world, void *shape, size_t size);
uint32_t	world_add_vertex(t_world *world, t_vertex *vertex);
uint32_t	world_add_accel_node(t_world *world, t_accel_node *accel_node);
uint32_t	world_add_accel_index(t_world *world, uint32_t accel_index);
uint32_t	world_add_accel_degenerate(t_world *world, uint32_t accel_degenerate);
uint32_t	world_add_tex(t_world *world, const t_tex *tex);
uint32_t	world_alloc_tex_data(t_world *world, size_t size);

void		world_load_ppm(t_world *world, t_tex *tex, const unsigned char *data, size_t len);

int64_t		world_get_material(const t_world *world, uint32_t index);

void		world_info_create(t_tree_info *tree, t_node_info *node, t_world *world);
void		world_info_init(t_tree_info *tree, t_node_info *node, t_world *world);
void		world_info_destroy(t_tree_info *tree, t_node_info *node);

void		material_init(t_material *mat);

/* voor daan */
uint32_t	world_max_depth(size_t prims_count);
uint32_t	new_node(t_world *world);
int			world_axis_side(const t_tree_info *tree, const t_split *split, uint32_t index);

const t_primitive	*get_primitive(const t_tree_info *info, size_t index);
t_bounds			get_bounds(const t_world *world, const t_primitive *primitive);
void				interior_node_init(t_node_info *parent_info, const t_node_info *above_info, const t_split *split);
void				leaf_node_init(t_node_info *node_info);
FLOAT				get_split_cost(const t_bounds bounds, const t_split *split, const uint32_t primitive_counts[2]);

#endif
