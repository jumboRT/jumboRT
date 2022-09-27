#include "world_impl.h"

#include "mat.h"
#include "shape.h"

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
	hash_mat(const GLOBAL t_material *mat, const GLOBAL t_world *world, t_seed *seed)
{
	(void) mat;
	(void) world;
	(void) seed;
	return (0);
}

uint64_t
	hash_tex(const GLOBAL t_tex *tex, const GLOBAL t_world *world, t_seed *seed)
{
	(void) tex;
	(void) world;
	(void) seed;
	return (0);
}

uint64_t
	hash_bxdf(const GLOBAL t_bxdf_any *bxdf, const GLOBAL t_world *world, t_seed *seed)
{
	(void) bxdf;
	(void) world;
	(void) seed;
	return (0);
}

uint64_t
	hash_sphere(const GLOBAL t_primitive *prim, const GLOBAL t_world *world, t_seed *seed)
{
	const GLOBAL t_shape_sphere	*shape;
	uint64_t					hash;

	(void) world;
	shape = (const GLOBAL t_shape_sphere *) prim;
	hash = 0;
	hash ^= hash_data(seed, &shape->pos, sizeof(shape->pos));
	hash ^= hash_data(seed, &shape->radius, sizeof(shape->radius));
	return (hash);
}

uint64_t
	hash_triangle(const GLOBAL t_primitive *prim, const GLOBAL t_world *world, t_seed *seed)
{
	const GLOBAL t_shape_triangle	*shape;
	uint64_t						hash;

	(void) world;
	shape = (const GLOBAL t_shape_triangle *) prim;
	hash = 0;
	hash ^= hash_data(seed, &shape->a, sizeof(shape->a));
	hash ^= hash_data(seed, &shape->b, sizeof(shape->b));
	hash ^= hash_data(seed, &shape->c, sizeof(shape->c));
	return (hash);
}

uint64_t
	hash_plane(const GLOBAL t_primitive *prim, const GLOBAL t_world *world, t_seed *seed)
{
	const GLOBAL t_shape_plane	*shape;
	uint64_t					hash;

	(void) world;
	shape = (const GLOBAL t_shape_plane *) prim;
	hash = 0;
	hash ^= hash_data(seed, &shape->plane.pos, sizeof(shape->plane.pos));
	hash ^= hash_data(seed, &shape->plane.normal, sizeof(shape->plane.normal));
	return (hash);
}

uint64_t
	hash_cylinder(const GLOBAL t_primitive *prim, const GLOBAL t_world *world, t_seed *seed)
{
	const GLOBAL t_shape_cylinder	*shape;
	uint64_t						hash;

	(void) world;
	shape = (const GLOBAL t_shape_cylinder *) prim;
	hash = 0;
	hash ^= hash_data(seed, &shape->cylinder.pos, sizeof(shape->cylinder.pos));
	hash ^= hash_data(seed, &shape->cylinder.dir, sizeof(shape->cylinder.dir));
	hash ^= hash_data(seed, &shape->cylinder.height, sizeof(shape->cylinder.height));
	hash ^= hash_data(seed, &shape->cylinder.radius, sizeof(shape->cylinder.radius));
	return (hash);
}

uint64_t
	hash_cone(const GLOBAL t_primitive *prim, const GLOBAL t_world *world, t_seed *seed)
{
	const GLOBAL t_shape_cone	*shape;
	uint64_t					hash;

	(void) world;
	shape = (const GLOBAL t_shape_cone *) prim;
	hash = 0;
	hash ^= hash_data(seed, &shape->cone.pos, sizeof(shape->cone.pos));
	hash ^= hash_data(seed, &shape->cone.dir, sizeof(shape->cone.dir));
	hash ^= hash_data(seed, &shape->cone.height, sizeof(shape->cone.height));
	hash ^= hash_data(seed, &shape->cone.angle, sizeof(shape->cone.angle));
	return (hash);
}

uint64_t
	hash_prim(const GLOBAL t_primitive *prim, const GLOBAL t_world *world, t_seed *seed)
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
	return (hash);
}

uint64_t
	hash_world(const GLOBAL t_world *world, int flags)
{
	uint64_t					index;
	const GLOBAL char			*primitives;
	const GLOBAL t_primitive	*primitive;
	uint64_t					hash;
	t_seed						seed;

	hash = 0;
	seed = 7549087012;
	primitives = (const GLOBAL char *) world->primitives;
	if (flags & RT_HASH_GEOMETRY)
	{
		index = 0;
		while (index < world->primitives_size)
		{
			primitive = (const GLOBAL t_primitive *) (primitives + index);
			hash ^= hash_prim(primitive, world, &seed);
			index += world_primitive_size(prim_type(primitive));
		}
		index = 0;
		while (index < world->vertices_count)
		{
			hash ^= hash_data(&seed, &world->vertices[index].pos, sizeof(world->vertices[index].pos));
			index += 1;
		}
	}
	if (flags & RT_HASH_MATERIALS)
	{
		index = 0;
		while (index < world->materials_count)
		{
			hash ^= hash_mat(&world->materials[index], world, &seed);
			index += 1;
		}
		index = 0;
		while (index < world->bxdfs_count)
		{
			hash ^= hash_bxdf(&world->bxdfs[index], world, &seed);
			index += 1;
		}
		index = 0;
		while (index < world->textures_count)
		{
			hash ^= hash_tex(&world->textures[index], world, &seed);
			index += 1;
		}
		index = 0;
		while (index < world->vertices_count)
		{
			hash ^= hash_data(&seed, &world->vertices[index].uv, sizeof(world->vertices[index].uv));
			hash ^= hash_data(&seed, &world->vertices[index].normal, sizeof(world->vertices[index].normal));
			index += 1;
		}
		hash ^= hash_data(&seed, world->texture_data, world->texture_data_size);
	}
	return (hash);
}
