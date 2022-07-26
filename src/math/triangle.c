#include "rtmath.h"

__attribute__ ((const))
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
	FLOAT	d00, d01, d11, d20, d21;
	FLOAT	denom;
	FLOAT	bc_u, bc_v, bc_w;

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
	hit->normal = normal;
	if (bc_v >= 0 && bc_w >= 0 && bc_v + bc_w <= 1)
	{
		hit->uv = vec2(
				vec_dot(vec(u(triangle.uvs[0]), u(triangle.uvs[1]), u(triangle.uvs[2]), 0.0), vec(bc_u, bc_v, bc_w, 0.0)),
				vec_dot(vec(v(triangle.uvs[0]), v(triangle.uvs[1]), v(triangle.uvs[2]), 0.0), vec(bc_u, bc_v, bc_w, 0.0)));
		if (is_smooth)
		{
			hit->normal = vec(
					vec_dot(vec(x(triangle.normals[0]), x(triangle.normals[1]), x(triangle.normals[2]), 0.0), vec(bc_u, bc_v, bc_w, 0.0)),
					vec_dot(vec(y(triangle.normals[0]), y(triangle.normals[1]), y(triangle.normals[2]), 0.0), vec(bc_u, bc_v, bc_w, 0.0)),
					vec_dot(vec(z(triangle.normals[0]), z(triangle.normals[1]), z(triangle.normals[2]), 0.0), vec(bc_u, bc_v, bc_w, 0.0)),
					0.0);
		}
		return (1);
	}
	return (0);
}
