#ifndef WORLD_IMPL_H
# define WORLD_IMPL_H

# include "world.h"
# include "state.h"
# include "vector.h"
# include "aabb.h"
# include "pool.h"

# include <stddef.h>

struct s_prim_info {
	uint32_t	index;
	t_bounds	bounds;
};

void		world_create(t_world *world);
void		world_destroy(t_world *world);
void		*world_reallog(void *ptr, uint64_t *capacity, uint64_t size);

uint32_t	world_add_material(t_world *world, t_material *material);
uint32_t	world_add_primitive(t_world *world, void *shape, size_t size);
uint32_t	world_add_vertex(t_world *world, t_vertex *vertex);
uint32_t	world_add_accel_node(t_world *world, t_accel_node *accel_node);
uint32_t	world_add_accel_index(t_world *world, uint32_t accel_index);
uint32_t	world_add_accel_degenerate(t_world *world,
				uint32_t accel_degenerate);
uint32_t	world_add_tex(t_world *world, const t_tex *tex);
uint32_t	world_alloc_tex_data(t_world *world, size_t size);
void		world_insert_bxdf(t_world *world, t_material *material, void *bxdf,
				size_t size);
uint32_t	world_add_light(t_world *world, uint32_t light);
uint32_t	world_add_leaf_data(t_world *world, t_leaf_data leaf_data);

void		world_load_bmp(t_world *world, t_tex *tex,
				const unsigned char *data, size_t len);

void		world_attach_ropes(t_world *world, t_bounds bounds);

void		material_init(t_material *mat, const t_world *world);
void		texture_init(t_tex *tex);

void		world_accel(t_world *world);
void		leaf_create(t_accel_node *leaf, const uint32_t *prim_indices,
				uint32_t prim_count, uint32_t *out_indices);
void		interior_create(t_accel_node *interior, uint32_t axis,
				uint32_t above_child, float offset);
#endif
