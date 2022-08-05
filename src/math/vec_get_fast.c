#include "rtmath.h"

#if defined RT_VECTORIZE

FLOAT
	x(t_vec v)
{
	t_vec_conv	conv;
	
	conv.vec = v;
	return (conv.elem[0]);
}

FLOAT
	y(t_vec v)
{
	t_vec_conv	conv;
	
	conv.vec = v;
	return (conv.elem[1]);
}

FLOAT
	z(t_vec v)
{
	t_vec_conv	conv;
	
	conv.vec = v;
	return (conv.elem[2]);
}

FLOAT
	w(t_vec v)
{
	t_vec_conv	conv;
	
	conv.vec = v;
	return (conv.elem[3]);
}

FLOAT
	u(t_vec2 v)
{
	t_vec2_conv	conv;

	conv.vec = v;
	return (conv.elem[0]);
}

FLOAT
	v(t_vec2 v)
{
	t_vec2_conv	conv;

	conv.vec = v;
	return (conv.elem[1]);
}

FLOAT
	xyz(t_vec v, int i)
{
	if (i == 0)
		return (x(v));
	else if (i == 1)
		return (y(v));
	else
		return (z(v));
}

int
	vec_eq(t_vec a, t_vec b)
{
	return (x(a) == x(b) && y(a) == y(b) && z(a) == z(b));
}

#endif
