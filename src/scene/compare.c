#include "scene.h"

#include <math.h>

/* https://gdbooks.gitbooks.io/3dcollisions/content/Chapter2/static_aabb_plane.html */
int
	box_plane_compare(t_vec plane_pos, t_vec plane_dir, t_vec a, t_vec b)
{
	t_vec	points[8];
	FLOAT	results[8];

	points[0] = vec_sub(a, plane_pos);
	points[1] = vec_sub(b, plane_pos);
	points[2] = points[0];
	points[2].v[X] = points[1].v[X];
	points[3] = points[0];
	points[3].v[Y] = points[1].v[Y];
	points[4] = points[0];
	points[4].v[Z] = points[1].v[Z];
	points[5] = points[1];
	points[5].v[X] = points[0].v[X];
	points[6] = points[1];
	points[6].v[Y] = points[0].v[Y];
	points[7] = points[1];
	points[7].v[Z] = points[0].v[Z];

	results[0] = vec_dot(points[0], plane_dir);
	results[1] = vec_dot(points[1], plane_dir);
	results[2] = vec_dot(points[2], plane_dir);
	results[3] = vec_dot(points[3], plane_dir);
	results[4] = vec_dot(points[4], plane_dir);
	results[5] = vec_dot(points[5], plane_dir);
	results[6] = vec_dot(points[6], plane_dir);
	results[7] = vec_dot(points[7], plane_dir);
	if (results[0] >= 0 && results[1] >= 0 && results[2] >= 0 && results[3] >= 0
			&& results[4] >= 0 && results[5] >= 0 && results[6] >= 0 && results[7] >= 0)
		return (1);
	else if (results[0] <= 0 && results[1] <= 0 && results[2] <= 0 && results[3] <= 0
			&& results[4] <= 0 && results[5] <= 0 && results[6] <= 0 && results[7] <= 0)
		return (-1);
	return (0);
}


int
	sphere_plane_compare(t_vec plane_pos, t_vec plane_dir, t_vec pos, FLOAT radius)
{
	t_vec	plane_sphere;
	t_vec	closest_sphere;
	FLOAT	f;

	plane_sphere = vec_sub(pos, plane_pos);
	f = vec_dot(plane_sphere, plane_dir);
	closest_sphere = vec_scale(plane_dir, f);
	if (vec_mag2(closest_sphere) >= radius * radius)
		return ((f > 0) - (f < 0));
	return (0);
}

