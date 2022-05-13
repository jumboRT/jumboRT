#include "math.h"

t_vec
	vec_cross(t_vec a, t_vec b)
{
	return ()
}

t_vec
	vec_add(t_vec a, t_vec b)
{
	return (vec(
		a.v[X] + b.v[X],
		a.v[Y] + b.v[Y],
		a.v[Z] + b.v[Z],
		a.v[W] + b.v[W]
	));
}

t_vec
	vec_sub(t_vec a, t_vec b)
{
	return (vec(
		a.v[X] - b.v[X],
		a.v[Y] - b.v[Y],
		a.v[Z] - b.v[Z],
		a.v[W] - b.v[W]
	));
}

t_vec
	vec_neg(t_vec a)
{
	return (vec(
		-a.v[X],
		-a.v[Y],
		-a.v[Z],
		-a.v[W]
	));
}

t_vec
	vec_norm(t_vec a)
{

}
