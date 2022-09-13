#include "world.h"

static t_sphere
	make_sphere(const GLOBAL t_world *world, const GLOBAL void *ptr)
{
	const GLOBAL t_shape_sphere	*sp;

	(void) world;
	sp = (const GLOBAL t_shape_sphere *) ptr;
	return (sphere(sp->pos, sp->radius));
}

static t_triangle
	make_triangle(const GLOBAL t_world *world, const GLOBAL void *ptr)
{
	const GLOBAL t_shape_triangle	*tr;
	const GLOBAL t_material			*mat;

	tr = (const GLOBAL t_shape_triangle *) ptr;
	mat = get_mat_const(world, prim_mat(&tr->base));
	return (triangle(
			world->vertices[tr->a].pos,
			world->vertices[tr->b].pos,
			world->vertices[tr->c].pos,
			world->vertices[tr->a].uv,
			world->vertices[tr->b].uv,
			world->vertices[tr->c].uv,
			world->vertices[tr->a].normal,
			world->vertices[tr->b].normal,
			world->vertices[tr->c].normal,
			(mat->flags & RT_MAT_SMOOTH) != 0));
}

int
	prim_intersect(const GLOBAL t_primitive *prim, const GLOBAL t_world *world, t_ray ray, float min, t_world_hit *hit)
{
	int	did_hit;

	did_hit = 0;
	if (prim_type(prim) == RT_SHAPE_SPHERE)
		did_hit = ray_sphere_intersect(ray, make_sphere(world, prim), min, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_TRIANGLE)
		did_hit = ray_triangle_intersect(ray, make_triangle(world, prim), min, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_PLANE)
		did_hit = ray_plane_intersect(ray, ((const GLOBAL t_shape_plane *) prim)->plane, min, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_CYLINDER)
		did_hit = ray_cylinder_intersect(ray, ((const GLOBAL t_shape_cylinder *) prim)->cylinder, min, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_CONE)
		did_hit = ray_cone_intersect(ray, ((const GLOBAL t_shape_cone *) prim)->cone, min, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_PARABOLOID)
		did_hit = ray_paraboloid_intersect(ray, ((const GLOBAL t_shape_paraboloid *) prim)->paraboloid, min, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_HYPERBOLOID)
		did_hit = ray_hyperboloid_intersect(ray, ((const GLOBAL t_shape_hyperboloid *) prim)->hyperboloid, min, &hit->hit);
	hit->prim = prim;
	return (did_hit);
}

void
	prim_hit_info(const GLOBAL t_primitive *prim, const GLOBAL t_world *world, t_ray ray, t_world_hit *hit)
{
	if (prim_type(prim) == RT_SHAPE_SPHERE)
		sphere_hit_info(ray, make_sphere(world, prim), &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_TRIANGLE)
		triangle_hit_info(ray, make_triangle(world, prim), &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_PLANE)
		plane_hit_info(ray, ((const GLOBAL t_shape_plane *) prim)->plane, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_CYLINDER)
		cylinder_hit_info(ray, ((const GLOBAL t_shape_cylinder *) prim)->cylinder, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_CONE)
		cone_hit_info(ray, ((const GLOBAL t_shape_cone *) prim)->cone, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_PARABOLOID)
		paraboloid_hit_info(ray, ((const GLOBAL t_shape_paraboloid *) prim)->paraboloid, &hit->hit);
	else if (prim_type(prim) == RT_SHAPE_HYPERBOLOID)
		hyperboloid_hit_info(ray, ((const GLOBAL t_shape_hyperboloid *) prim)->hyperboloid, &hit->hit);
}

