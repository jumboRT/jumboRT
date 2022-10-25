/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   world_impl.h                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:54 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:54 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORLD_IMPL_H
# define WORLD_IMPL_H

# include "world.h"
# include "state.h"
# include "vector.h"
# include "aabb.h"
# include "pool.h"

# include <stddef.h>

struct s_prim_info {
	t_uint32	index;
	t_bounds	bounds;
};

void		world_create(t_world *world);
void		world_destroy(t_world *world);
void		*world_reallog(void *ptr, t_uint64 *capacity, t_uint64 size);

t_uint32	world_add_material(t_world *world, t_material *material);
t_uint32	world_add_primitive(t_world *world, void *shape, size_t size);
t_uint32	world_add_vertex(t_world *world, t_vertex *vertex);
t_uint32	world_add_accel_node(t_world *world, t_accel_node *accel_node);
t_uint32	world_add_accel_index(t_world *world, t_uint32 accel_index);
t_uint32	world_add_accel_degenerate(t_world *world,
				t_uint32 accel_degenerate);
t_uint32	world_add_tex(t_world *world, const t_tex *tex);
t_uint32	world_alloc_tex_data(t_world *world, size_t size);
void		world_insert_bxdf(t_world *world, t_material *material, void *bxdf,
				size_t size);
t_uint32	world_add_light(t_world *world, t_uint32 light);
t_uint32	world_add_leaf_data(t_world *world, t_leaf_data leaf_data);

void		world_load_bmp(t_world *world, t_tex *tex,
				const unsigned char *data, size_t len);

void		world_attach_ropes(t_world *world, t_bounds bounds);

void		material_init(t_material *mat, const t_world *world);
void		texture_init(t_tex *tex);

void		world_accel(t_world *world);
void		leaf_create(t_accel_node *leaf, const t_uint32 *prim_indices,
				t_uint32 prim_count, t_uint32 *out_indices);
void		interior_create(t_accel_node *interior, t_uint32 axis,
				t_uint32 above_child, float offset);
#endif
