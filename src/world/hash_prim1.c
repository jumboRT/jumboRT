/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   hash_prim1.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:28 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:28 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "hash.h"

t_uint64
	hash_sphere(const GLOBAL t_primitive *prim,
			const GLOBAL t_world *world, t_seed *seed)
{
	const GLOBAL t_shape_sphere	*shape;
	t_uint64					hash;

	(void) world;
	shape = (const GLOBAL t_shape_sphere *) prim;
	hash = 0;
	hash ^= hash_data(seed, &shape->pos, sizeof(shape->pos));
	hash ^= hash_data(seed, &shape->radius, sizeof(shape->radius));
	return (hash);
}

t_uint64
	hash_triangle(const GLOBAL t_primitive *prim,
			const GLOBAL t_world *world, t_seed *seed)
{
	const GLOBAL t_shape_triangle	*shape;
	t_uint64						hash;

	(void) world;
	shape = (const GLOBAL t_shape_triangle *) prim;
	hash = 0;
	hash ^= hash_data(seed, &shape->a, sizeof(shape->a));
	hash ^= hash_data(seed, &shape->b, sizeof(shape->b));
	hash ^= hash_data(seed, &shape->c, sizeof(shape->c));
	return (hash);
}

t_uint64
	hash_plane(const GLOBAL t_primitive *prim,
			const GLOBAL t_world *world, t_seed *seed)
{
	const GLOBAL t_shape_plane	*shape;
	t_uint64					hash;

	(void) world;
	shape = (const GLOBAL t_shape_plane *) prim;
	hash = 0;
	hash ^= hash_data(seed, &shape->plane.pos, sizeof(shape->plane.pos));
	hash ^= hash_data(seed, &shape->plane.normal, sizeof(shape->plane.normal));
	return (hash);
}

t_uint64
	hash_cylinder(const GLOBAL t_primitive *prim,
			const GLOBAL t_world *world, t_seed *seed)
{
	const GLOBAL t_shape_cylinder	*shape;
	t_uint64						hash;

	(void) world;
	shape = (const GLOBAL t_shape_cylinder *) prim;
	hash = 0;
	hash ^= hash_data(seed, &shape->cylinder.pos,
			sizeof(shape->cylinder.pos));
	hash ^= hash_data(seed, &shape->cylinder.dir,
			sizeof(shape->cylinder.dir));
	hash ^= hash_data(seed, &shape->cylinder.height,
			sizeof(shape->cylinder.height));
	hash ^= hash_data(seed, &shape->cylinder.radius,
			sizeof(shape->cylinder.radius));
	return (hash);
}

t_uint64
	hash_cone(const GLOBAL t_primitive *prim,
			const GLOBAL t_world *world, t_seed *seed)
{
	const GLOBAL t_shape_cone	*shape;
	t_uint64					hash;

	(void) world;
	shape = (const GLOBAL t_shape_cone *) prim;
	hash = 0;
	hash ^= hash_data(seed, &shape->cone.pos, sizeof(shape->cone.pos));
	hash ^= hash_data(seed, &shape->cone.dir, sizeof(shape->cone.dir));
	hash ^= hash_data(seed, &shape->cone.height, sizeof(shape->cone.height));
	hash ^= hash_data(seed, &shape->cone.angle, sizeof(shape->cone.angle));
	return (hash);
}
