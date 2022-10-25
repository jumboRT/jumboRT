/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   hash_prim2.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:31 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:31 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "hash.h"

t_uint64
	hash_paraboloid(const GLOBAL t_primitive *prim,
			const GLOBAL t_world *world, t_seed *seed)
{
	const GLOBAL t_shape_paraboloid	*shape;
	t_uint64						hash;

	(void) world;
	shape = (const GLOBAL t_shape_paraboloid *) prim;
	hash = 0;
	hash_data(seed, &shape->paraboloid.a, sizeof(shape->paraboloid.a));
	hash_data(seed, &shape->paraboloid.b, sizeof(shape->paraboloid.b));
	return (hash);
}

t_uint64
	hash_hyperboloid(const GLOBAL t_primitive *prim,
			const GLOBAL t_world *world, t_seed *seed)
{
	const GLOBAL t_shape_hyperboloid	*shape;
	t_uint64							hash;

	(void) world;
	shape = (const GLOBAL t_shape_hyperboloid *) prim;
	hash = 0;
	hash_data(seed, &shape->hyperboloid.a, sizeof(shape->hyperboloid.a));
	hash_data(seed, &shape->hyperboloid.b, sizeof(shape->hyperboloid.b));
	hash_data(seed, &shape->hyperboloid.c, sizeof(shape->hyperboloid.c));
	return (hash);
}

t_uint64
	hash_point(const GLOBAL t_primitive *prim,
			const GLOBAL t_world *world, t_seed *seed)
{
	const GLOBAL t_shape_point	*shape;
	t_uint64					hash;

	(void) world;
	shape = (const GLOBAL t_shape_point *) prim;
	hash = 0;
	hash ^= hash_data(seed, &shape->pos, sizeof(shape->pos));
	return (hash);
}
