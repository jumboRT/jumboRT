#include "world.h"

#include "util.h"

#include <math.h>

static inline t_bounds get_bounds_triangle(const t_world *world, const t_shape_triangle *triangle) {
	t_vec	points[3];

	points[0] = get_vertex(world, triangle->a);
	points[1] = get_vertex(world, triangle->b);
	points[2] = get_vertex(world, triangle->c);
	return (bounds(
				vec_min(points[0], vec_min(points[1], points[2])),
				vec_max(points[0], vec_max(points[1], points[2]))));
}

static t_bounds get_bounds_cylinder(const t_shape_cylinder *cylinder) {
	FLOAT	radius;
	FLOAT	dot;
	t_vec	top;
	t_vec	dif;
	t_vec	box;

	radius = cylinder->cylinder.radius;
	dif = vec_scale(cylinder->cylinder.dir, cylinder->cylinder.height);
	top = vec_add(cylinder->cylinder.pos, dif);
	dot = cylinder->cylinder.height * cylinder->cylinder.height;
	box = vec_scale(vec(
			sqrt(1.0 - x(dif) * x(dif) / dot),
			sqrt(1.0 - y(dif) * y(dif) / dot),
			sqrt(1.0 - z(dif) * z(dif) / dot),
			0.0), radius);
	return (bounds(
				vec_min(vec_sub(cylinder->cylinder.pos, box), vec_sub(top, box)),
				vec_max(vec_add(cylinder->cylinder.pos, box), vec_add(top, box))));
}

static inline t_bounds get_bounds_sphere(const t_shape_sphere *sphere) {
	return (bounds(
				vec_sub(sphere->pos, vec(sphere->radius, sphere->radius, sphere->radius, 0.0)),
				vec_add(sphere->pos, vec(sphere->radius, sphere->radius, sphere->radius, 0.0))));
}

static t_bounds get_bounds_cone(const t_shape_cone *shape) {
	FLOAT	radius;
	FLOAT	dot;
	t_vec	top;
	t_vec	dif;
	t_vec	box;

	radius = tan(shape->cone.angle) * shape->cone.height;
	dif = vec_scale(shape->cone.dir, shape->cone.height);
	top = vec_add(shape->cone.pos, dif);
	dot = shape->cone.height * shape->cone.height;
	box = vec_scale(vec(
			sqrt(1.0 - x(dif) * x(dif) / dot),
			sqrt(1.0 - y(dif) * y(dif) / dot),
			sqrt(1.0 - z(dif) * z(dif) / dot),
			0.0), radius);
	return (bounds(
				vec_min(shape->cone.pos, vec_sub(top, box)),
				vec_max(shape->cone.pos, vec_add(top, box))));
}

t_bounds prim_bounds(const t_primitive *primitive, const t_world *world) {
	if (prim_type(primitive) == RT_SHAPE_TRIANGLE) {
		return (get_bounds_triangle(world, (const t_shape_triangle *) primitive));
	} else if (prim_type(primitive) == RT_SHAPE_SPHERE) {
		return (get_bounds_sphere((const t_shape_sphere *) primitive));
	} else if (prim_type(primitive) == RT_SHAPE_CYLINDER) {
		return (get_bounds_cylinder((const t_shape_cylinder *) primitive));
	} else if (prim_type(primitive) == RT_SHAPE_CONE) {
		return (get_bounds_cone((const t_shape_cone *) primitive));
	}
	rt_assert(0, "unimplemented shape in get_bounds");
	return (bounds_0());
}

