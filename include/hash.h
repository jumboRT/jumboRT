#ifndef HASH_H
# define HASH_H

# include "world.h"
# include "shape.h"
# include "tex.h"

# include <stdlib.h>

uint64_t	hash_sphere(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
uint64_t	hash_triangle(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
uint64_t	hash_plane(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
uint64_t	hash_cylinder(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
uint64_t	hash_cone(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
uint64_t	hash_paraboloid(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
uint64_t	hash_hyperboloid(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
uint64_t	hash_point(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);

uint64_t	hash_bsdf(const GLOBAL t_bsdf *bsdf,
				const GLOBAL t_world *world, t_seed *seed);
uint64_t	hash_filter(const GLOBAL t_filter *filter,
				const GLOBAL t_world *world, t_seed *seed);
uint64_t	hash_mat(const GLOBAL t_material *mat,
				const GLOBAL t_world *world, t_seed *seed);
uint64_t	hash_bxdf_int(const GLOBAL t_bxdf_any *bxdf,
				const GLOBAL t_world *world, t_seed *seed);
uint64_t	hash_bxdf(const GLOBAL t_bxdf_any *bxdf,
				const GLOBAL t_world *world, t_seed *seed);

uint64_t	hash_salt(t_seed *seed, uint64_t salt);
uint64_t	hash_data(t_seed *seed, const void *data_ptr, size_t size);
uint64_t	hash_tex(const GLOBAL t_tex *tex,
				const GLOBAL t_world *world, t_seed *seed);
uint64_t	hash_prim(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_seed *seed);
uint64_t	hash_prim_size(uint32_t shape, t_seed *seed);

uint64_t	hash_salt(t_seed *seed, uint64_t salt);
uint64_t	hash_data(t_seed *seed, const void *data_ptr, size_t size);
uint64_t	hash_world(const GLOBAL t_world *world, int flags);

#endif
