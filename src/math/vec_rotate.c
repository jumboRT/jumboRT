#include "rtmath.h"

t_vec
    vec_rotate(t_vec axis, t_vec v, FLOAT angle)
{
    return (vec_add(
                vec_add(
                    vec_scale(v, rt_cos(angle)), 
                    vec_scale(vec_cross(axis, v), rt_sin(angle))), 
                vec_scale(axis, 
                    vec_dot(axis, v) * (1.0 - rt_cos(angle)))));
}

void
	vec_angles(t_vec v, FLOAT *xr, FLOAT *yr)
{
	*xr = rt_atan2(z(v), y(v)) - RT_PI / 2;
	*yr = rt_atan2(z(vec_rotate(vec_x(1), v, -*xr)), x(v)) - RT_PI / 2;
}

t_vec
	vec_rotate_fwd(t_vec v, FLOAT xr, FLOAT yr)
{
	v = vec_rotate(vec_y(-1), v, yr);
	v = vec_rotate(vec_x(1), v, xr);
	return (v);
}

t_vec
	vec_rotate_bwd(t_vec v, FLOAT xr, FLOAT yr)
{
	v = vec_rotate(vec_x(1), v, -xr);
	v = vec_rotate(vec_y(-1), v, -yr);
	return (v);
}

