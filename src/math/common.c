#include "rtmath.h"

t_vec
	vec(FLOAT x, FLOAT y, FLOAT z, FLOAT w)
{
	t_vec	result;

	result.v[X] = x;
	result.v[Y] = y;
	result.v[Z] = z;
	result.v[W] = w;
	return (result);
}

t_mat
	mat(t_vec a, t_vec b, t_vec c, t_vec d)
{
	t_mat	result;

	result.rows[0] = a;
	result.rows[1] = b;
	result.rows[2] = c;
	result.rows[3] = d;
	return (result);
}

t_vec
	row(t_mat m, size_t i)
{
	return (m.rows[i]);
}

t_vec
	col(t_mat m, size_t i)
{
	return (vec(
			m.rows[0].v[i],
			m.rows[1].v[i],
			m.rows[2].v[i],
			m.rows[3].v[i]));
}
