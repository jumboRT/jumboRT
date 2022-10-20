#include "rtmath.h"

#if defined RT_VECTORIZE

float
	x(t_vec v)
{
	t_vec_conv	conv;

	conv.vec = v;
	return (conv.elem[0]);
}

float
	y(t_vec v)
{
	t_vec_conv	conv;

	conv.vec = v;
	return (conv.elem[1]);
}

float
	z(t_vec v)
{
	t_vec_conv	conv;

	conv.vec = v;
	return (conv.elem[2]);
}

float
	w(t_vec v)
{
	t_vec_conv	conv;

	conv.vec = v;
	return (conv.elem[3]);
}

float
	xyz(t_vec v, int i)
{
	if (i == 0)
		return (x(v));
	else if (i == 1)
		return (y(v));
	else
		return (z(v));
}

#endif
