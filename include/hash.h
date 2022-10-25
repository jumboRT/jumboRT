/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   hash.h                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:49 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:49 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_H
# define HASH_H

# include "world.h"
# include "shape.h"
# include "tex.h"

# include <stdlib.h>

t_uint64	hash_sphere(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
t_uint64	hash_triangle(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
t_uint64	hash_plane(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
t_uint64	hash_cylinder(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
t_uint64	hash_cone(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
t_uint64	hash_paraboloid(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
t_uint64	hash_hyperboloid(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
t_uint64	hash_point(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);

t_uint64	hash_bsdf(const GLOBAL t_bsdf *bsdf,
				const GLOBAL t_world *world, t_seed *seed);
t_uint64	hash_filter(const GLOBAL t_filter *filter,
				const GLOBAL t_world *world, t_seed *seed);
t_uint64	hash_mat(const GLOBAL t_material *mat,
				const GLOBAL t_world *world, t_seed *seed);
t_uint64	hash_bxdf_int(const GLOBAL t_bxdf_any *bxdf,
				const GLOBAL t_world *world, t_seed *seed);
t_uint64	hash_bxdf(const GLOBAL t_bxdf_any *bxdf,
				const GLOBAL t_world *world, t_seed *seed);

t_uint64	hash_salt(t_seed *seed, t_uint64 salt);
t_uint64	hash_data(t_seed *seed, const void *data_ptr, size_t size);
t_uint64	hash_tex(const GLOBAL t_tex *tex,
				const GLOBAL t_world *world, t_seed *seed);
t_uint64	hash_prim(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
t_uint64	hash_prim_size(t_uint32 shape, t_seed *seed);

t_uint64	hash_salt(t_seed *seed, t_uint64 salt);
t_uint64	hash_data(t_seed *seed, const void *data_ptr, size_t size);
t_uint64	hash_world(const GLOBAL t_world *world, int flags);

#endif
