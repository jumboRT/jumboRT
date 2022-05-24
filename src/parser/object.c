#include "parser.h"

#include "util.h"
#include "scene.h"
#include <libft.h>

t_entity
	*rt_sphere(const char **line, char **error)
{
	t_sphere	sphere;

	*line = rt_pos(*line, error, &sphere.pos);
	*line = rt_float(*line, error, &sphere.diameter);
	*line = rt_material(*line, error, &sphere.mat);
	if (*line == NULL)
		return (NULL);
	sphere.base.vt = sphere_vt();
	return (rt_memdup(&sphere, sizeof(sphere)));
}

t_entity
	*rt_plane(const char **line, char **error)
{
	t_plane		plane;

	*line = rt_pos(*line, error, &plane.pos);
	*line = rt_norm_vec(*line, error, &plane.dir);
	*line = rt_material(*line, error, &plane.mat);
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
	*line = rt_material(*line, error, &cyl.mat);
	if (*line == NULL)
		return (NULL);
	cyl.base.vt = cylinder_vt();
	return (rt_memdup(&cyl, sizeof(cyl)));
}
