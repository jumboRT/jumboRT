#include "rtmath.h"
#include "util.h"

t_vec
	vec_hnorm(t_vec a)
{
	FLOAT	mag;

	mag = a.v[W];
	rt_assert(mag != 0, "cannot normalize vector with 0 magnitude");
	return (vec_scale(a, (FLOAT) 1.0 / mag));
}

t_vec
	vec_mul(t_vec a, t_mat b)
{
	return (vec(
			vec_dot(a, row(b, 0)),
			vec_dot(a, row(b, 1)),
			vec_dot(a, row(b, 2)),
			vec_dot(a, row(b, 3))));
}

t_mat
	mat_mul(t_mat a, t_mat b)
{
	b = transpose(b);
	return (mat(
			vec_mul(row(a, 0), b),
			vec_mul(row(a, 1), b),
			vec_mul(row(a, 2), b),
			vec_mul(row(a, 3), b)));
}

t_vec
	vec_colormul(t_vec a, t_vec b)
{
	return (vec(
			a.v[X] * b.v[X],
			a.v[Y] * b.v[Y],
			a.v[Z] * b.v[Z],
			a.v[W] * b.v[W]));
}
