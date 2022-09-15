#include "rtmath.h"
#include "sample.h"

t_triangle
	triangle(t_vec v0, t_vec v1, t_vec v2, t_vec2 uv0, t_vec2 uv1, t_vec2 uv2, t_vec n0, t_vec n1, t_vec n2, int is_smooth)
{
	t_triangle	result;

	result.vertices[0] = v0;
	result.vertices[1] = v1;
	result.vertices[2] = v2;
	result.uvs[0] = uv0;
	result.uvs[1] = uv1;
	result.uvs[2] = uv2;
	result.normals[0] = n0;
	result.normals[1] = n1;
	result.normals[2] = n2;
	result.is_smooth = is_smooth;
	return (result);
}

void
	triangle_hit_info(t_ray ray, t_triangle triangle, t_hit *hit)
{
	t_vec	v0, v1;
	t_vec2	duv02;
	t_vec2	duv12;
	t_vec	dp02;
	t_vec	dp12;
	t_vec	tmp;
	float	determinant;
	int		degenerate;
	t_vec	bary;

	(void) ray;
	v0 = vec_sub(triangle.vertices[1], triangle.vertices[0]);
	v1 = vec_sub(triangle.vertices[2], triangle.vertices[0]);
	duv02 = vec2_sub(triangle.uvs[0], triangle.uvs[2]);
	duv12 = vec2_sub(triangle.uvs[1], triangle.uvs[2]);
	dp02 = vec_sub(triangle.vertices[0], triangle.vertices[2]);
	dp12 = vec_sub(triangle.vertices[1], triangle.vertices[2]);
	determinant = (u(duv02) * v(duv12) - v(duv02) * u(duv12));
	degenerate = rt_abs(determinant) < RT_TINY_VAL;
	hit->dpdu = vec_0();
	hit->dpdv = vec_0();
	/* TODO check if triangle has UV's before reading degenerate. Use of unitialized value */
	if (!degenerate)
	{
		determinant = 1.0 / determinant;
		hit->dpdu = vec_scale(vec_sub(vec_scale(dp02, v(duv12)), vec_scale(dp12, v(duv02))), determinant);
		hit->dpdv = vec_scale(vec_add(vec_scale(dp02, -u(duv12)), vec_scale(dp12, u(duv02))), determinant);
	}
	/* When I changed this below to an else statement, it was WAAAAY faster. Probably something to do with branching! Try to 
	 * optimize this! */
	if (degenerate || vec_mag2(vec_cross(hit->dpdu, hit->dpdv)) == 0.0)	
	{
		tmp = vec_norm(vec_cross(v1, v0));
		if (rt_abs(x(tmp)) > rt_abs(y(tmp)))
			hit->dpdu = vec_scale(vec(-z(tmp), 0.0, x(tmp), 0.0), rt_sqrt(x(tmp) * x(tmp) + z(tmp) * z(tmp)));
		else
			hit->dpdu = vec_scale(vec(0.0, z(tmp), -y(tmp), 0.0), rt_sqrt(y(tmp) * y(tmp) + z(tmp) * z(tmp)));
		hit->dpdv = vec_cross(tmp, hit->dpdu);
	}
	if (vec_mag2(vec_cross(hit->dpdu, hit->dpdv)) <= 0.0001)
	{
		hit->dpdu = vec_z(1);
		hit->dpdv = vec_x(1);
	}
	bary = vec(hit->ctx.tr.bc_u, hit->ctx.tr.bc_v, hit->ctx.tr.bc_w, 0);
	hit->uv = vec2(
			vec_dot(vec(u(triangle.uvs[0]), u(triangle.uvs[1]), u(triangle.uvs[2]), 0.0), bary),
			vec_dot(vec(v(triangle.uvs[0]), v(triangle.uvs[1]), v(triangle.uvs[2]), 0.0), bary));
	if (triangle.is_smooth)
	{
		hit->shading_normal = vec(
				vec_dot(vec(x(triangle.normals[0]), x(triangle.normals[1]), x(triangle.normals[2]), 0.0), bary),
				vec_dot(vec(y(triangle.normals[0]), y(triangle.normals[1]), y(triangle.normals[2]), 0.0), bary),
				vec_dot(vec(z(triangle.normals[0]), z(triangle.normals[1]), z(triangle.normals[2]), 0.0), bary),
				0.0);
	}
}

int
	ray_triangle_intersect(t_ray ray, t_triangle triangle, float min, t_hit *hit)
{
	t_vec	normal;
	t_vec	v0, v1, v2;
	float	d00, d01, d11, d20, d21;
	float	denom;

	normal = vec_norm2(vec_cross(
				vec_sub(triangle.vertices[2], triangle.vertices[0]),
				vec_sub(triangle.vertices[1], triangle.vertices[2])));
	if (!ray_plane_intersect(ray, plane(triangle.vertices[2], normal), min, hit))
		return (0);
	v0 = vec_sub(triangle.vertices[1], triangle.vertices[0]);
	v1 = vec_sub(triangle.vertices[2], triangle.vertices[0]);
	v2 = vec_sub(hit->pos, triangle.vertices[0]);

	d00 = vec_dot(v0, v0);
	d01 = vec_dot(v0, v1);
	d11 = vec_dot(v1, v1);
	d20 = vec_dot(v2, v0);
	d21 = vec_dot(v2, v1);

	denom = 1.0 / ((d00 * d11) - (d01 * d01));
	hit->ctx.tr.bc_v = ((d11 * d20) - (d01 * d21)) * denom; 
	hit->ctx.tr.bc_w = ((d00 * d21) - (d01 * d20)) * denom;
	hit->ctx.tr.bc_u = 1 - hit->ctx.tr.bc_v - hit->ctx.tr.bc_w;
	return (hit->ctx.tr.bc_v >= 0 && hit->ctx.tr.bc_w >= 0 && hit->ctx.tr.bc_v + hit->ctx.tr.bc_w <= 1);
}

t_vec
	triangle_sample(t_triangle triangle, GLOBAL t_context *ctx)
{
	t_vec	v0;
	t_vec	v1;
	float	u0;
	float	u1;

	v0 = vec_sub(triangle.vertices[1], triangle.vertices[0]);
	v1 = vec_sub(triangle.vertices[2], triangle.vertices[0]);
	u0 = rt_random_float(&ctx->seed);
	u1 = rt_random_float(&ctx->seed);
	if (u0 + u1 > 1)
	{
		u0 = 1 - u0;
		u1 = 1 - u1;
	}
	v0 = vec_scale(v0, u0);
	v1 = vec_scale(v1, u1);
	return (vec_add(vec_add(v0, v1), triangle.vertices[0]));
}

float
	triangle_area(t_triangle triangle)
{
	t_vec	v0;
	t_vec	v1;
	float	dot;
	float	sin_theta;
	float	area;

	v0 = vec_sub(triangle.vertices[1], triangle.vertices[0]);
	v1 = vec_sub(triangle.vertices[2], triangle.vertices[0]);
	dot = vec_dot(v0, v1);
	sin_theta = rt_sqrt(1 - dot * dot);
	area = 0.5f * vec_mag(v0) * vec_mag(v1) * sin_theta;
	return (area);
}

