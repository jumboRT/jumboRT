#include "rtmath.h"

t_vec2
	vec_change_basis2(t_vec v, t_vec tangent, t_vec bit_tangent)
{
	return (vec2(vec_dot(v, tangent), vec_dot(v, bit_tangent)));
}
