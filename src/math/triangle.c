#include "rtmath.h"

t_triangle
	triangle(t_vec v0, t_vec v1, t_vec v2, t_vec2 uv0, t_vec2 uv1, t_vec2 uv2, t_vec n0, t_vec n1, t_vec n2)
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
	return (result);
}

int
	ray_triangle_intersect(t_ray ray, t_triangle triangle, FLOAT min, t_hit *hit, int is_smooth)
{
	t_vec	pos, normal;
	t_vec	v0, v1, v2;
	t_vec2	duv02, duv12;
	t_vec	dp02, dp12;
	t_vec	tmp;
	FLOAT	d00, d01, d11, d20, d21;
	FLOAT	denom;
	FLOAT	invdet;
	FLOAT	bc_u, bc_v, bc_w;
	int		degenerate;

	normal = vec_norm2(vec_cross(
				vec_sub(triangle.vertices[2], triangle.vertices[0]),
				vec_sub(triangle.vertices[1], triangle.vertices[2])));
	if (!ray_plane_intersect(ray, plane(triangle.vertices[2], normal), min, hit))
		return (0);
	pos = ray_at(ray, hit->t);
	v0 = vec_sub(triangle.vertices[1], triangle.vertices[0]);
	v1 = vec_sub(triangle.vertices[2], triangle.vertices[0]);
	v2 = vec_sub(pos, triangle.vertices[0]);

	d00 = vec_dot(v0, v0);
	d01 = vec_dot(v0, v1);
	d11 = vec_dot(v1, v1);
	d20 = vec_dot(v2, v0);
	d21 = vec_dot(v2, v1);

	denom = 1.0 / ((d00 * d11) - (d01 * d01));
	bc_v = ((d11 * d20) - (d01 * d21)) * denom; 
	bc_w = ((d00 * d21) - (d01 * d20)) * denom;
	bc_u = 1 - bc_v - bc_w;
	hit->pos = pos;
	hit->geometric_normal = normal;
	hit->shading_normal = normal;
	if (bc_v >= 0 && bc_w >= 0 && bc_v + bc_w <= 1)
	{
		hit->uv = vec2(
				vec_dot(vec(u(triangle.uvs[0]), u(triangle.uvs[1]), u(triangle.uvs[2]), 0.0), vec(bc_u, bc_v, bc_w, 0.0)),
				vec_dot(vec(v(triangle.uvs[0]), v(triangle.uvs[1]), v(triangle.uvs[2]), 0.0), vec(bc_u, bc_v, bc_w, 0.0)));
		duv02 = vec2_sub(triangle.uvs[0], triangle.uvs[2]);
		duv12 = vec2_sub(triangle.uvs[1], triangle.uvs[2]);
		dp02 = vec_sub(triangle.vertices[0], triangle.vertices[2]);
		dp12 = vec_sub(triangle.vertices[1], triangle.vertices[2]);
		invdet = (u(duv02) * v(duv12) - v(duv02) * u(duv12));
		degenerate = rt_abs(invdet) > 0.0001;
		hit->dpdu = vec_0();
		hit->dpdv = vec_0();
		/* TODO check if triangle has UV's before reading degenerate. Use of unitialized value */
		if (degenerate)
		{
			invdet = 1.0 / invdet;
			hit->dpdu = vec_scale(vec_sub(vec_scale(dp02, v(duv12)), vec_scale(dp12, v(duv02))), invdet);
			hit->dpdv = vec_scale(vec_add(vec_scale(dp02, -u(duv12)), vec_scale(dp12, u(duv02))), invdet);
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
		if (vec_mag2(vec_cross(hit->dpdu, hit->dpdv)) <= 0.01)
		{
			hit->dpdu = vec_z(1);
			hit->dpdv = vec_x(1);
		}
		if (is_smooth)
		{
			hit->shading_normal = vec(
					vec_dot(vec(x(triangle.normals[0]), x(triangle.normals[1]), x(triangle.normals[2]), 0.0), vec(bc_u, bc_v, bc_w, 0.0)),
					vec_dot(vec(y(triangle.normals[0]), y(triangle.normals[1]), y(triangle.normals[2]), 0.0), vec(bc_u, bc_v, bc_w, 0.0)),
					vec_dot(vec(z(triangle.normals[0]), z(triangle.normals[1]), z(triangle.normals[2]), 0.0), vec(bc_u, bc_v, bc_w, 0.0)),
					0.0);
		}
		return (1);
	}
	return (0);
}
