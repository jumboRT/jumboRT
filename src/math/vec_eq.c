#include "rtmath.h"

int
	vec_eq(t_vec a, t_vec b)
{
	return (x(a) == x(b) && y(a) == y(b) && z(a) == z(b) && w(a) == w(b));
}

int
	vec2_eq(t_vec2 a, t_vec2 b)
{
	return (u(a) == u(b) && v(a) == v(b));
}
