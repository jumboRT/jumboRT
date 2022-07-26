#include "rtmath.h"

#if defined RT_VECTORIZE

__attribute__ ((const))
FLOAT
	x(t_vec v)
{
	t_vec_conv	conv;
	
	conv.vec = v;
	return (conv.elem[0]);
}

__attribute__ ((const))
FLOAT
	y(t_vec v)
{
	t_vec_conv	conv;
	
	conv.vec = v;
	return (conv.elem[1]);
}

__attribute__ ((const))
FLOAT
	z(t_vec v)
{
	t_vec_conv	conv;
	
	conv.vec = v;
	return (conv.elem[2]);
}

__attribute__ ((const))
FLOAT
	w(t_vec v)
{
	t_vec_conv	conv;
	
	conv.vec = v;
	return (conv.elem[3]);
}

__attribute__ ((const))
FLOAT
	u(t_vec2 v)
{
	t_vec2_conv	conv;

	conv.vec = v;
	return (conv.elem[0]);
}

__attribute__ ((const))
FLOAT
	v(t_vec2 v)
{
	t_vec2_conv	conv;

	conv.vec = v;
	return (conv.elem[1]);
}

__attribute__ ((const))
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

__attribute__ ((const))
int
	vec_eq(t_vec a, t_vec b)
{
	return (x(a) == x(b) && y(a) == y(b) && z(a) == z(b));
}

#endif
