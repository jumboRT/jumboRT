/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   new_work.c                                     #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:11 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:11 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

#if RT_BONUS
# include "util.h"
# include "world_impl.h"
# include "parser.h"

# include <libft.h>
# include <ft_printf.h>

static int
	rt_is_simple_update(struct s_net_worker *worker,
			struct s_sjob_request request)
{
	return (worker->work != NULL
		&& ft_strcmp(worker->opts.scene_file, request.scene_file.str) == 0
		&& ft_strcmp(worker->opts.key, request.scene_key.str) == 0
		&& (t_uint64) worker->opts.width == request.width
		&& (t_uint64) worker->opts.height == request.height);
}

static void
	rt_simple_update(struct s_net_worker *worker,
			struct s_sjob_request request, t_cam_params p)
{
	t_state	*state;

	state = worker->work->state;
	rt_work_lock(worker->work);
	state->world->render_mode = request.render_mode;
	state->world->batch_size = request.batch_size;
	state->world->trace_batch_size = request.trace_batch_size;
	camera_set(state->world, &state->world->camera, p);
	worker->work->work_size = 0;
	rt_work_unlock(worker->work);
}

static void
	rt_destroy_work(t_work *work)
{
	work_pause(work);
	work_update_stop(work);
	world_destroy(work->state->world);
	work_destroy(work);
	rt_free(work->state->world);
	rt_free(work->state);
	rt_free(work);
	rt_free((char *) work->opts->scene_file);
	rt_free((char *) work->opts->key);
}

static void
	rt_full_update(struct s_net_worker *worker,
			struct s_sjob_request request, t_cam_params p)
{
	t_state	*state;

	if (worker->work != NULL)
		rt_destroy_work(worker->work);
	worker->work = rt_malloc(sizeof(*worker->work));
	state = rt_malloc(sizeof(*state));
	state->image = NULL;
	state->world = rt_malloc(sizeof(*state->world));
	world_create(state->world);
	state->world->render_mode = request.render_mode;
	state->world->batch_size = request.batch_size;
	state->world->trace_batch_size = request.trace_batch_size;
	state->world->img_meta.width = request.width;
	state->world->img_meta.height = request.height;
	state->world->img_meta.start_x = 0;
	state->world->img_meta.start_y = 0;
	state->world->img_meta.end_x = request.width;
	state->world->img_meta.end_y = request.height;
	world_load(state->world, request.scene_file.str, request.scene_key.str);
	world_accel(state->world);
	camera_set(state->world, &state->world->camera, p);
	work_create(worker->work, state, &worker->opts, (union u_client *) worker);
	work_update_start(worker->work);
	worker->opts.width = request.width;
	worker->opts.height = request.height;
	worker->opts.start_x = 0;
	worker->opts.start_y = 0;
	worker->opts.end_x = request.width;
	worker->opts.end_y = request.height;
	worker->opts.scene_file = ft_strdup(request.scene_file.str);
	worker->opts.key = ft_strdup(request.scene_key.str);
	work_resume(worker->work);
}

void
	rt_create_new_work(struct s_net_worker *worker,
			struct s_sjob_request request)
{
	t_cam_params	p;

	ft_printf("NEW JOB %s:%s\n", request.scene_file.str, request.scene_key.str);
	p.org = request.cam_pos;
	p.dir = request.cam_dir;
	p.fov = request.cam_fov;
	p.focus = request.cam_focus;
	p.blur = request.cam_blur;
	if (rt_is_simple_update(worker, request))
	{
		rt_simple_update(worker, request, p);
		return ;
	}
	rt_full_update(worker, request, p);
}
#endif
