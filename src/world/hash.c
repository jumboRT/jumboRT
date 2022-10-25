/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   hash.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:28 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:28 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "hash.h"
#include "mat.h"
#include "shape.h"

static t_uint64
	hash_geometry(const GLOBAL t_world *world, int flags, t_seed *seed)
{
	t_uint64					index;
	const GLOBAL char			*primitives;
	const GLOBAL t_primitive	*primitive;
	t_uint64					hash;

	(void) flags;
	hash = 0;
	primitives = (const GLOBAL char *) world->primitives;
	index = 0;
	while (index < world->primitives_size)
	{
		primitive = (const GLOBAL t_primitive *)(primitives + index);
		hash ^= hash_prim(primitive, world, seed);
		index += world_primitive_size(prim_type(primitive));
	}
	index = 0;
	while (index < world->vertices_count)
	{
		hash ^= hash_data(seed, &world->vertices[index].pos,
				sizeof(world->vertices[index].pos));
		index += 1;
	}
	return (hash);
}

static t_uint64
	hash_materials_int(const GLOBAL t_world *world, t_seed *seed)
{
	t_uint64	index;
	t_uint64	hash;

	hash = 0;
	index = 0;
	while (index < world->materials_count)
	{
		hash ^= hash_mat(&world->materials[index], world, seed);
		index += 1;
	}
	index = 0;
	while (index < world->bxdfs_count)
	{
		hash ^= hash_bxdf(&world->bxdfs[index], world, seed);
		index += 1;
	}
	index = 0;
	while (index < world->textures_count)
	{
		hash ^= hash_tex(&world->textures[index], world, seed);
		index += 1;
	}
	return (hash);
}

static t_uint64
	hash_materials(const GLOBAL t_world *world, int flags, t_seed *seed)
{
	t_uint64	index;
	t_uint64	hash;

	(void) flags;
	hash = 0;
	hash ^= hash_data(seed, &world->flags, sizeof(world->flags));
	hash ^= hash_data(seed, &world->ambient_mat, sizeof(world->ambient_mat));
	hash ^= hash_data(seed, &world->ambient_dist, sizeof(world->ambient_dist));
	hash ^= hash_materials_int(world, seed);
	index = 0;
	while (index < world->vertices_count)
	{
		hash ^= hash_data(seed, &world->vertices[index].uv,
				sizeof(world->vertices[index].uv));
		hash ^= hash_data(seed, &world->vertices[index].normal,
				sizeof(world->vertices[index].normal));
		index += 1;
	}
	hash ^= hash_data(seed, world->texture_data, world->texture_data_size);
	return (hash);
}

t_uint64
	hash_world(const GLOBAL t_world *world, int flags)
{
	t_uint64	hash;
	t_seed		seed;

	seed = 7549087012;
	hash = 0;
	hash ^= hash_prim_size(RT_SHAPE_SPHERE, &seed);
	hash ^= hash_prim_size(RT_SHAPE_TRIANGLE, &seed);
	hash ^= hash_prim_size(RT_SHAPE_PLANE, &seed);
	hash ^= hash_prim_size(RT_SHAPE_CYLINDER, &seed);
	hash ^= hash_prim_size(RT_SHAPE_CONE, &seed);
	hash ^= hash_prim_size(RT_SHAPE_PARABOLOID, &seed);
	hash ^= hash_prim_size(RT_SHAPE_HYPERBOLOID, &seed);
	hash ^= hash_prim_size(RT_SHAPE_POINT, &seed);
	if (flags & RT_HASH_GEOMETRY)
		hash ^= hash_geometry(world, flags, &seed);
	if (flags & RT_HASH_MATERIALS)
		hash ^= hash_materials(world, flags, &seed);
	return (hash);
}
