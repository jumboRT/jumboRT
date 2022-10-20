#include "world.h"
#include "rtmath.h"
#include "shape.h"
#include "mat.h"

t_sphere
	make_sphere(const GLOBAL t_world *world, const GLOBAL t_void *ptr)
{
	const GLOBAL t_shape_sphere	*sp;

	(void) world;
	sp = (const GLOBAL t_shape_sphere *) ptr;
	return (sphere(sp->pos, sp->radius));
}

t_triangle
	make_triangle(const GLOBAL t_world *world, const GLOBAL t_void *ptr)
{
	const GLOBAL t_shape_triangle	*tr;
	const GLOBAL t_material			*mat;
	t_triangle						result;

	tr = (const GLOBAL t_shape_triangle *) ptr;
	mat = get_mat_const(world, prim_mat(&tr->base));
	result.vertices[0] = world->vertices[tr->a].pos;
	result.vertices[1] = world->vertices[tr->b].pos;
	result.vertices[2] = world->vertices[tr->c].pos;
	result.uvs[0] = world->vertices[tr->a].uv;
	result.uvs[1] = world->vertices[tr->b].uv;
	result.uvs[2] = world->vertices[tr->c].uv;
	result.normals[0] = world->vertices[tr->a].normal;
	result.normals[1] = world->vertices[tr->a].normal;
	result.normals[2] = world->vertices[tr->a].normal;
	result.is_smooth = (mat->flags & RT_MAT_SMOOTH) != 0;
	return (result);
}
