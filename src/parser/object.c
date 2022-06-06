#include "parser.h"

#include "util.h"
#include "scene.h"
#include <libft.h>
#include <math.h>


#include <stdio.h>

t_entity
	*rt_triangle(const char **line, char **error)
{
	t_triangle	triangle;

	*line = rt_pos(*line, error, &triangle.pos0);
	*line = rt_pos(*line, error, &triangle.pos1);
	*line = rt_pos(*line, error, &triangle.pos2);
	triangle.mat = rt_material(line, error);
	if (*line == NULL)
		return (NULL);
	triangle.base.vt = triangle_vt();
	return (rt_memdup(&triangle, sizeof(triangle)));
}

t_entity
	*rt_sphere(const char **line, char **error)
{
	t_sphere	sphere;

	*line = rt_pos(*line, error, &sphere.pos);
	*line = rt_float(*line, error, &sphere.diameter);
	sphere.mat = rt_material(line, error);
	if (*line == NULL)
		return (NULL);
	sphere.base.vt = sphere_vt();
	return (rt_memdup(&sphere, sizeof(sphere)));
}

t_entity
	*rt_cone(const char **line, char **error)
{
	t_cone	cone;

	*line = rt_pos(*line, error, &cone.pos);
	*line = rt_norm_vec(*line, error, &cone.dir);
	*line = rt_float(*line, error, &cone.angle);
	*line = rt_float(*line, error, &cone.height);
	cone.mat = rt_material(line, error);
	if (*line == NULL)
		return (NULL);
	cone.base.vt = cone_vt();
	cone.costheta2 = pow(cos(cone.angle), 2.0);
	cone.r = tan(cone.angle) * cone.height;
	return (rt_memdup(&cone, sizeof(cone)));
}

t_entity
	*rt_plane(const char **line, char **error)
{
	t_plane		plane;

	*line = rt_pos(*line, error, &plane.pos);
	*line = rt_norm_vec(*line, error, &plane.dir);
	plane.mat = rt_material(line, error);
	if (*line == NULL)
		return (NULL);
	plane.base.vt = plane_vt();
	return (rt_memdup(&plane, sizeof(plane)));
}

t_entity
	*rt_cylinder(const char **line, char **error)
{
	t_cylinder	cyl;

	*line = rt_pos(*line, error, &cyl.pos);
	*line = rt_norm_vec(*line, error, &cyl.dir);
	*line = rt_float(*line, error, &cyl.diameter);
	*line = rt_float(*line, error, &cyl.height);
	cyl.mat = rt_material(line, error);
	if (*line == NULL)
		return (NULL);
	cyl.base.vt = cylinder_vt();
	return (rt_memdup(&cyl, sizeof(cyl)));
}
