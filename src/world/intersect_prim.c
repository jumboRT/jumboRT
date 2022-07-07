#include "world.h"

static inline int
	sphere_intersect(const GLOBAL void *ptr, t_ray ray, FLOAT min, t_hit *hit)
{
	const GLOBAL t_shape_sphere	*sp;

	sp = (const GLOBAL t_shape_sphere *) ptr;
	return (ray_sphere_intersect(
				ray,
				sphere(
					sp->pos,
					sp->radius),
				min,
				hit));
}

static inline int
	triangle_intersect(const GLOBAL t_world *world, const GLOBAL void *ptr, t_ray ray, FLOAT min, t_hit *hit)
{
	const GLOBAL t_shape_triangle	*tr;

	tr = (const GLOBAL t_shape_triangle *) ptr;
	return (ray_triangle_intersect(
				ray,
				triangle(
					world->vertices[tr->a].pos,
					world->vertices[tr->b].pos,
					world->vertices[tr->c].pos),
				min,
				hit));
}

static inline int
	plane_intersect(const GLOBAL void *ptr, t_ray ray, FLOAT min, t_hit *hit)
{
	const GLOBAL t_shape_plane	*pl;

	pl = (const GLOBAL t_shape_plane *) ptr;
	return (ray_plane_intersect(ray, pl->plane, min, hit));
}

static inline int
	cylinder_intersect(const GLOBAL void *ptr, t_ray ray, FLOAT min, t_hit *hit)
{
	const GLOBAL t_shape_cylinder *cl;

	cl = (const GLOBAL t_shape_cylinder *) ptr;
	return (ray_cylinder_intersect(ray, cl->cylinder, min, hit));
}

static inline int
	cone_intersect(const GLOBAL void *ptr, t_ray ray, FLOAT min, t_hit *hit)
{
	const GLOBAL t_shape_cone	*co;

	co = (const GLOBAL t_shape_cone *) ptr;
	return (ray_cone_intersect(ray, co->cone, min, hit));
}

int
	prim_intersect(const GLOBAL t_primitive *prim, const GLOBAL t_world *world, t_ray ray, FLOAT min, t_hit *hit)
{
	if (prim->shape_type == RT_SHAPE_SPHERE)
		return (sphere_intersect(prim, ray, min, hit));
	else if (prim->shape_type == RT_SHAPE_TRIANGLE)
		return (triangle_intersect(world, prim, ray, min, hit));
	else if (prim->shape_type == RT_SHAPE_PLANE)
		return (plane_intersect(prim, ray, min, hit));
	else if (prim->shape_type == RT_SHAPE_CYLINDER)
		return (cylinder_intersect(prim, ray, min, hit));
	else if (prim->shape_type == RT_SHAPE_CONE)
		return (cone_intersect(prim, ray, min, hit));
	return (0);
}
