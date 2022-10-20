#include "hash.h"

uint64_t
	hash_salt(t_seed *seed, uint64_t salt)
{
	if (*seed + salt == 0)
		*seed = *seed - 1;
	else
		*seed += salt;
	return (rt_random(seed));
}

uint64_t
	hash_data(t_seed *seed, const void *data_ptr, size_t size)
{
	const unsigned char	*data;
	size_t				index;
	uint64_t			hash;

	data = data_ptr;
	index = 0;
	hash = 0;
	while (index + 3 < size)
	{
		hash ^= hash_salt(seed, *(uint32_t *) &data[index]);
		index += 4;
	}
	while (index < size)
	{
		hash ^= hash_salt(seed, data[index]);
		index += 1;
	}
	return (hash);
}

uint64_t
	hash_tex(const GLOBAL t_tex *tex, const GLOBAL t_world *world, t_seed *seed)
{
	uint64_t	hash;

	(void) world;
	hash = hash_salt(seed, tex->type);
	if (tex->type == RT_TEX_COLOR)
	{
		hash ^= hash_data(seed, &tex->a.color, sizeof(tex->a.color));
	}
	else if (tex->type == RT_TEX_TEXTURE)
	{
		hash ^= hash_data(seed, &tex->a.tex.width, sizeof(tex->a.tex.width));
		hash ^= hash_data(seed, &tex->a.tex.height, sizeof(tex->a.tex.height));
		hash ^= hash_data(seed, &tex->a.tex.offset, sizeof(tex->a.tex.offset));
	}
	else if (tex->type == RT_TEX_CHECKER)
	{
		hash ^= hash_data(seed, &tex->a.checker.odd_color,
				sizeof(tex->a.checker.odd_color));
		hash ^= hash_data(seed, &tex->a.checker.even_color,
				sizeof(tex->a.checker.even_color));
	}
	return (hash);
}

uint64_t
	hash_prim(const GLOBAL t_primitive *prim,
			const GLOBAL t_world *world, t_seed *seed)
{
	uint64_t	hash;

	hash = hash_salt(seed, prim_type(prim));
	if (prim_type(prim) == RT_SHAPE_SPHERE)
		hash ^= hash_sphere(prim, world, seed);
	else if (prim_type(prim) == RT_SHAPE_TRIANGLE)
		hash ^= hash_triangle(prim, world, seed);
	else if (prim_type(prim) == RT_SHAPE_PLANE)
		hash ^= hash_plane(prim, world, seed);
	else if (prim_type(prim) == RT_SHAPE_CYLINDER)
		hash ^= hash_cylinder(prim, world, seed);
	else if (prim_type(prim) == RT_SHAPE_CONE)
		hash ^= hash_cone(prim, world, seed);
	else if (prim_type(prim) == RT_SHAPE_PARABOLOID)
		hash ^= hash_paraboloid(prim, world, seed);
	else if (prim_type(prim) == RT_SHAPE_HYPERBOLOID)
		hash ^= hash_hyperboloid(prim, world, seed);
	else if (prim_type(prim) == RT_SHAPE_POINT)
		hash ^= hash_point(prim, world, seed);
	return (hash);
}

uint64_t
	hash_prim_size(uint32_t shape, t_seed *seed)
{
	return (hash_data(seed, &shape, sizeof(shape)));
}
