#include "rtmath.h"
#include "sample.h"

void
	triangle_hit_info_bary(t_ray unused, t_triangle triangle, t_hit *hit)
{
	t_vec	bary;

	(void) unused;
	bary = vec(hit->u_ctx.s_tr.bc_u, hit->u_ctx.s_tr.bc_v,
			hit->u_ctx.s_tr.bc_w, 0);
	hit->uv = vec2(
			vec_dot(vec(u(triangle.uvs[0]), u(triangle.uvs[1]),
					u(triangle.uvs[2]), 0.0), bary),
			vec_dot(vec(v(triangle.uvs[0]), v(triangle.uvs[1]),
					v(triangle.uvs[2]), 0.0), bary));
	if (triangle.is_smooth)
	{
		hit->shading_normal = vec(
				vec_dot(vec(x(triangle.normals[0]), x(triangle.normals[1]),
						x(triangle.normals[2]), 0.0), bary),
				vec_dot(vec(y(triangle.normals[0]), y(triangle.normals[1]),
						y(triangle.normals[2]), 0.0), bary),
				vec_dot(vec(z(triangle.normals[0]), z(triangle.normals[1]),
						z(triangle.normals[2]), 0.0), bary),
				0.0);
		hit->dpdu = vec_cross(hit->shading_normal, hit->dpdv);
		hit->dpdv = vec_cross(hit->shading_normal, hit->dpdu);
	}
}

void
	triangle_hit_info(t_ray ray, t_triangle triangle, t_hit *hit)
{
	t_vec2	duv02;
	t_vec2	duv12;
	t_vec	dp02;
	t_vec	dp12;
	float	determinant;

	duv02 = vec2_sub(triangle.uvs[0], triangle.uvs[2]);
	duv12 = vec2_sub(triangle.uvs[1], triangle.uvs[2]);
	dp02 = vec_sub(triangle.vertices[0], triangle.vertices[2]);
	dp12 = vec_sub(triangle.vertices[1], triangle.vertices[2]);
	determinant = (u(duv02) * v(duv12) - v(duv02) * u(duv12));
	if (rt_abs(determinant) < RT_TINY_VAL)
	{
		hit->dpdu = vec_tangent(hit->shading_normal);
		hit->dpdv = vec_cross(hit->shading_normal, hit->dpdu);
	}
	else
	{
		determinant = 1.0 / determinant;
		hit->dpdu = vec_scale(vec_sub(vec_scale(dp02, v(duv12)),
					vec_scale(dp12, v(duv02))), determinant);
		hit->dpdv = vec_scale(vec_add(vec_scale(dp02, -u(duv12)),
					vec_scale(dp12, u(duv02))), determinant);
	}
	triangle_hit_info_bary(ray, triangle, hit);
}

int
	triangle_intersect(t_ray ray, t_triangle triangle, float min, t_hit *hit)
{
	t_vec	n012[4];
	float	val[6];

	n012[0] = vec_norm2(vec_cross(
				vec_sub(triangle.vertices[2], triangle.vertices[0]),
				vec_sub(triangle.vertices[1], triangle.vertices[2])));
	if (!plane_intersect(ray, plane(triangle.vertices[2], n012[0]), min, hit))
		return (0);
	n012[1] = vec_sub(triangle.vertices[1], triangle.vertices[0]);
	n012[2] = vec_sub(triangle.vertices[2], triangle.vertices[0]);
	n012[3] = vec_sub(hit->pos, triangle.vertices[0]);
	val[0] = vec_dot(n012[1], n012[1]);
	val[1] = vec_dot(n012[1], n012[2]);
	val[2] = vec_dot(n012[2], n012[2]);
	val[3] = vec_dot(n012[3], n012[1]);
	val[4] = vec_dot(n012[3], n012[2]);
	val[5] = 1.0 / ((val[0] * val[2]) - (val[1] * val[1]));
	hit->u_ctx.s_tr.bc_v = ((val[2] * val[3]) - (val[1] * val[4])) * val[5];
	hit->u_ctx.s_tr.bc_w = ((val[0] * val[4]) - (val[1] * val[3])) * val[5];
	hit->u_ctx.s_tr.bc_u = 1 - hit->u_ctx.s_tr.bc_v - hit->u_ctx.s_tr.bc_w;
	return (hit->u_ctx.s_tr.bc_v >= 0 && hit->u_ctx.s_tr.bc_w >= 0
		&& hit->u_ctx.s_tr.bc_v + hit->u_ctx.s_tr.bc_w <= 1);
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
	dot = vec_dot(vec_norm(v0), vec_norm(v1));
	sin_theta = rt_sqrt(1 - dot * dot);
	area = 0.5f * vec_mag(v0) * vec_mag(v1) * sin_theta;
	return (area);
}
