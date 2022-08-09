#include "rtmath.h"

t_vec
	vec_tangent(t_vec v)
{
	if (rt_abs(z(v)) >= 0.999)
		return (vec3(z(v), -x(v), 0.0));
	return (vec3(y(v), -x(v), 0.0));
}
