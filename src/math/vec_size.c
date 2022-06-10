#include "rtmath.h"

#if !defined RT_SSE
# include <math.h>

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
	return (sqrt(vec_mag2(a)));
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

__attribute__ ((const))
t_vec
	vec_clamp(t_vec v, FLOAT min, FLOAT max)
{
	if (v.x < min)
		v.x = min;
	else if (v.x > max)
		v.x = max;
	if (v.y < min)
		v.y = min;
	else if (v.y > max)
		v.y = max;
	if (v.z < min)
		v.y = min;
	else if (v.z > max)
		v.z = max;
	return (v);
}
#endif
