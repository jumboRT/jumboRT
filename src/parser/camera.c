#include "parser.h"

#include "util.h"
#include <libft.h>
#include <ft_printf.h>

t_entity
	*rt_camera(const char **line, char **error)
{
	t_camera	camera;

	*line = rt_pos(*line, error, &camera.pos);
	*line = rt_norm_vec(*line, error, &camera.dir);
	*line = rt_float(*line, error, &camera.fov);
	if (*line == NULL)
		return (NULL);
	if (camera.fov < 0 || camera.fov > 180)
	{
		ft_asprintf(error, "Fov of camera must be in range [0-180]\n");
		return (NULL);
	}
	camera.base.vt = camera_vt();
	return (rt_memdup(&camera, sizeof(camera)));
}
