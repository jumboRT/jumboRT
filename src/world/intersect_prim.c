#include "world.h"
#include "sample.h"
#include "shape.h"
#include "mat.h"

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
	prim_intersect(const GLOBAL t_primitive *prim, const GLOBAL t_world *world,
			t_ray ray, float min, t_world_hit *hit)
{
	hit->prim = prim;
	if (prim_type(prim) == RT_SHAPE_SPHERE)
		return (ray_sphere_intersect(ray, make_sphere(world, prim), min, &hit->hit));
	else if (prim_type(prim) == RT_SHAPE_TRIANGLE)
		return (ray_triangle_intersect(ray, make_triangle(world, prim), min, &hit->hit));
	else if (prim_type(prim) == RT_SHAPE_PLANE)
		return (ray_plane_intersect(ray, ((const GLOBAL t_shape_plane *) prim)->plane, min, &hit->hit));
	else if (prim_type(prim) == RT_SHAPE_CYLINDER)
		return (ray_cylinder_intersect(ray, ((const GLOBAL t_shape_cylinder *) prim)->cylinder, min, &hit->hit));
	else if (prim_type(prim) == RT_SHAPE_CONE)
		return (ray_cone_intersect(ray, ((const GLOBAL t_shape_cone *) prim)->cone, min, &hit->hit));
	else if (prim_type(prim) == RT_SHAPE_PARABOLOID)
		return (ray_paraboloid_intersect(ray, ((const GLOBAL t_shape_paraboloid *) prim)->paraboloid, min, &hit->hit));
	else if (prim_type(prim) == RT_SHAPE_HYPERBOLOID)
		return (ray_hyperboloid_intersect(ray, ((const GLOBAL t_shape_hyperboloid *) prim)->hyperboloid, min, &hit->hit));
	return (0);
}

void
	prim_hit_info(const GLOBAL t_primitive *prim, const GLOBAL t_world *world, t_ray ray, t_world_hit *hit)
{
	hit->mat = get_mat_const(world, prim_mat(hit->prim));
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

t_vec
	prim_sample(const GLOBAL t_primitive *prim, const GLOBAL t_world *world, GLOBAL t_context *ctx)
{
	if (prim_type(prim) == RT_SHAPE_SPHERE)
		return (sphere_sample(make_sphere(world, prim), ctx));
	else if (prim_type(prim) == RT_SHAPE_TRIANGLE)
		return (triangle_sample(make_triangle(world, prim), ctx));
	else if (prim_type(prim) == RT_SHAPE_CYLINDER)
		return (cylinder_sample(((const GLOBAL t_shape_cylinder *) prim)->cylinder, ctx));
	else if (prim_type(prim) == RT_SHAPE_CONE)
		return (cone_sample(((const GLOBAL t_shape_cone *) prim)->cone, ctx));
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
		return (cylinder_area(((const GLOBAL t_shape_cylinder *) prim)->cylinder));
	else if (prim_type(prim) == RT_SHAPE_CONE)
		return (cone_area(((const GLOBAL t_shape_cone *) prim)->cone));
	else if (prim_type(prim) == RT_SHAPE_POINT)
		return (RT_PI);
	return (0);
}

