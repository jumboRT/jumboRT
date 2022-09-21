#include "rtmath.h"

t_vec
	sphere_to_cart(float theta, float phi)
{
	return (vec3(rt_cos(phi) * rt_sin(theta),
			rt_cos(phi) * rt_sin(theta),
			rt_cos(theta)));
}
