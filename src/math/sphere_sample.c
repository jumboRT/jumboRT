#include "rtmath.h"
#include "sample.h"

t_vec
	sphere_sample(t_sphere sphere, GLOBAL t_context *ctx)
{
	return (vec_add(sphere.pos,
			vec_scale(vec_norm(rt_random_in_sphere(&ctx->seed)),
				sphere.radius)));
}

float
	sphere_area(t_sphere sphere)
{
	return (4.0f * RT_PI * sphere.radius * sphere.radius);
}
