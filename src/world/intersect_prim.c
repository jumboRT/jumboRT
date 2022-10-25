/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   intersect_prim.c                               #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:28 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:28 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include "sample.h"
#include "shape.h"
#include "mat.h"

int
	prim_intersect(const GLOBAL t_primitive *prim, const GLOBAL t_world *world,
			t_ray_min ray, t_world_hit *hit)
{
	hit->prim = prim;
	if (prim_type(prim) == RT_SHAPE_SPHERE)
		return (sphere_intersect(ray.ray,
				make_sphere(world, prim), ray.min, &hit->hit));
	else if (prim_type(prim) == RT_SHAPE_TRIANGLE)
		return (triangle_intersect(ray.ray,
				make_triangle(world, prim), ray.min, &hit->hit));
	else if (prim_type(prim) == RT_SHAPE_PLANE)
		return (plane_intersect(ray.ray, ((const GLOBAL t_shape_plane *)
					prim)->plane, ray.min, &hit->hit));
	else if (prim_type(prim) == RT_SHAPE_CYLINDER)
		return (cylinder_intersect(ray.ray, ((const GLOBAL t_shape_cylinder *)
					prim)->cylinder, ray.min, &hit->hit));
	else if (prim_type(prim) == RT_SHAPE_CONE)
		return (cone_intersect(ray.ray, ((const GLOBAL t_shape_cone *)
					prim)->cone, ray.min, &hit->hit));
	else if (prim_type(prim) == RT_SHAPE_PARABOLOID)
		return (para_intersect(ray.ray, ((const GLOBAL t_shape_paraboloid *)
					prim)->paraboloid, ray.min, &hit->hit));
	else if (prim_type(prim) == RT_SHAPE_HYPERBOLOID)
		return (hyper_intersect(ray.ray, ((const GLOBAL t_shape_hyperboloid *)
					prim)->hyperboloid, ray.min, &hit->hit));
	return (0);
}

void
	prim_hit_info(const GLOBAL t_primitive *prim, const GLOBAL t_world *world,
			t_ray ray, t_world_hit *hit)
{
	hit->mat = get_mat_const(world, prim_mat(hit->prim));
	if (prim_type(prim) == RT_SHAPE_SPHERE)
		sphere_hit_info(ray, make_sphere(world, prim), &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_TRIANGLE)
		triangle_hit_info(ray, make_triangle(world, prim), &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_PLANE)
		plane_hit_info(ray, ((const GLOBAL t_shape_plane *)
				prim)->plane, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_CYLINDER)
		cylinder_hit_info(ray, ((const GLOBAL t_shape_cylinder *)
				prim)->cylinder, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_CONE)
		cone_hit_info(ray, ((const GLOBAL t_shape_cone *)
				prim)->cone, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_PARABOLOID)
		para_hit_info(ray, ((const GLOBAL t_shape_paraboloid *)
				prim)->paraboloid, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_HYPERBOLOID)
		hyper_hit_info(ray, ((const GLOBAL t_shape_hyperboloid *)
				prim)->hyperboloid, &hit->hit);
}

t_vec
	prim_sample(const GLOBAL t_primitive *prim, const GLOBAL t_world *world,
			GLOBAL t_context *ctx)
{
	if (prim_type(prim) == RT_SHAPE_SPHERE)
		return (sphere_sample(make_sphere(world, prim), ctx));
	else if (prim_type(prim) == RT_SHAPE_TRIANGLE)
		return (triangle_sample(make_triangle(world, prim), ctx));
	else if (prim_type(prim) == RT_SHAPE_CYLINDER)
		return (cylinder_sample(((const GLOBAL t_shape_cylinder *)
					prim)->cylinder, ctx));
	else if (prim_type(prim) == RT_SHAPE_CONE)
		return (cone_sample(((const GLOBAL t_shape_cone *)
					prim)->cone, ctx));
	else if (prim_type(prim) == RT_SHAPE_POINT)
		return (((const GLOBAL t_shape_point *) prim)->pos);
	return (vec_0());
}

float
	prim_area(const GLOBAL t_primitive *prim, const GLOBAL t_world *world)
{
	if (prim_type(prim) == RT_SHAPE_SPHERE)
		return (sphere_area(make_sphere(world, prim)));
	else if (prim_type(prim) == RT_SHAPE_TRIANGLE)
		return (triangle_area(make_triangle(world, prim)));
	else if (prim_type(prim) == RT_SHAPE_CYLINDER)
		return (cylinder_area(((const GLOBAL t_shape_cylinder *)
					prim)->cylinder));
	else if (prim_type(prim) == RT_SHAPE_CONE)
		return (cone_area(((const GLOBAL t_shape_cone *)
					prim)->cone));
	else if (prim_type(prim) == RT_SHAPE_POINT)
		return (RT_PI);
	return (0);
}
