#include "rtmath.h"

#if defined RT_VECTORIZE

t_vec
	vec_abs(t_vec v)
{
	return (v * __builtin_convertvector((v < vec_0()) | 1, t_vec));
}

#endif
