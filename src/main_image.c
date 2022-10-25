/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main_image.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:05 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:05 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "perf.h"
#include "util.h"

#include <libft.h>

static void
	main_image_save(t_work *work, const char *image_file)
{
	const char	*ext;

	mutex_lock(&work->state_mtx);
	ext = ft_strrchr(image_file, '.');
	if (ext && ft_strcmp(ext, ".pfm") == 0)
		rt_write_pfm(image_file, work->state->image);
	else
		rt_write_ppm(image_file, work->state->image);
	mutex_unlock(&work->state_mtx);
}

void
	main_image(t_work *work, const char *image_file)
{
	t_perf		perf;

	setup_sighandlers();
	main_update_start(work);
	perf_start(&perf);
	work_resume(work);
	while (1)
	{
		mutex_lock(&work->state_mtx);
		if (work->work_progress >= work->work_size || should_exit(0))
		{
			mutex_unlock(&work->state_mtx);
			break ;
		}
		mutex_unlock(&work->state_mtx);
		rt_usleep(10000);
	}
	perf_split(&perf, "draw image");
	main_image_save(work, image_file);
	perf_split(&perf, "save image");
	rt_exit(work);
}
