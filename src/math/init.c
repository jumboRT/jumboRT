#include "rtmath.h"

t_mat
	diagonal(FLOAT v)
{
	return (mat(
			vec(v, 0, 0, 0),
			vec(0, v, 0, 0),
			vec(0, 0, v, 0),
			vec(0, 0, 0, v)));
}

t_mat
	transpose(t_mat m)
{
	return (mat(
			col(m, 0),
			col(m, 1),
			col(m, 2),
			col(m, 3)));
}
