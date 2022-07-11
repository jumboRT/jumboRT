#include "rtmath.h"

#if !defined RT_VECTORIZE

__attribute__ ((const))
FLOAT
	vec_mag2(t_vec a)
{
	return (vec_dot(a, a));
}

__attribute__ ((const))
FLOAT
	vec_mag(t_vec a)
{
	return (rt_sqrt(vec_mag2(a)));
}

__attribute__ ((const))
t_vec
	vec_norm(t_vec v)
{
	return (vec_scale(v, (FLOAT) 1.0 / vec_mag(v)));
}

/* Safe version of vec_norm that doesn't divide by 0 */
__attribute__ ((const))
t_vec
	vec_norm2(t_vec v)
{
	FLOAT	len;

	len = vec_mag(v);
	if (len == 0)
		return (vec_z(1.0));
	return (vec_scale(v, (FLOAT) 1.0 / len));
}

#endif
