#include "rtmath.h"

#include "util.h"
#include <math.h>

t_vec
	vec_cross(t_vec a, t_vec b)
{
	return (vec(
			(a.v[Y] * b.v[Z]) - (a.v[Z] * b.v[Y]),
			(a.v[Z] * b.v[Y]) - (a.v[X] * b.v[Z]),
			(a.v[X] * b.v[Y]) - (a.v[Y] * b.v[X]),
			(FLOAT) 0));
}

t_vec
	vec_add(t_vec a, t_vec b)
{
	return (vec(
 			a.v[X] + b.v[X],
			a.v[Y] + b.v[Y],
			a.v[Z] + b.v[Z],
			a.v[W] + b.v[W]));
}

t_vec
	vec_sub(t_vec a, t_vec b)
{
	return (vec_add(a, vec_neg(b)));
}

t_vec
	vec_neg(t_vec a)
{
	return (vec(
			-a.v[X],
			-a.v[Y],
			-a.v[Z],
			-a.v[W]));
}

t_vec
	vec_norm(t_vec a)
{
	FLOAT	mag;

	mag = vec_mag(a);
	rt_assert(mag != 0, "cannot normalize vector with 0 magnitude");
	return (vec_scale(a, (FLOAT) 1.0 / mag));
}

/* https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula */
t_vec
    vec_rotate(t_vec axis, t_vec v, FLOAT angle)
{
    return (vec_add(
                vec_add(
                    vec_scale(v, cos(angle)), 
                    vec_scale(vec_cross(axis, v), sin(angle))), 
                vec_scale(axis, 
                    vec_dot(axis, v) * (1.0 - cos(angle)))));
}
