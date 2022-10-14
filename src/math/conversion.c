#include "rtmath.h"

float
	rt_degtorad(float deg)
{
	return (deg * (RT_PI / 180));
}

t_vec
	sphere_to_cart(float theta, float phi)
{
	return (vec3(rt_cos(phi) * rt_sin(theta),
			rt_sin(phi) * rt_sin(theta),
			rt_cos(theta)));
}

