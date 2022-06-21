#include "rtmath.h"

#if defined RT_VECTORIZE

__attribute__ ((const))
FLOAT
	x(t_vec v)
{
	t_vec_conv conv;
	
	conv.vec = v;
	return (conv.elem[0]);
}

__attribute__ ((const))
FLOAT
	y(t_vec v)
{
	t_vec_conv conv;
	
	conv.vec = v;
	return (conv.elem[1]);
}

__attribute__ ((const))
FLOAT
	z(t_vec v)
{
	t_vec_conv conv;
	
	conv.vec = v;
	return (conv.elem[2]);
}

__attribute__ ((const))
FLOAT
	w(t_vec v)
{
	t_vec_conv conv;
	
	conv.vec = v;
	return (conv.elem[3]);
}
#endif
