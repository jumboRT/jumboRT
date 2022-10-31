/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main_window.c                                  #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:20 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:20 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ft_printf.h>

#ifndef RT_JOINC

static int
	rt_loop(void *ctx)
{
	t_work	*work;
	int		update_flag;

	work = ctx;
	if (should_exit(0))
		rt_exit(work);
	mutex_lock(&work->update_mtx);
	update_flag = work->update_flag;
	work->update_flag = -1;
	mutex_unlock(&work->update_mtx);
	mutex_lock(&work->state_mtx);
	win_put(&work->state->win, work->state->image);
	mutex_unlock(&work->state_mtx);
	mutex_lock(&work->update_mtx);
	work->update_flag = update_flag;
	cond_broadcast(&work->update_cnd);
	mutex_unlock(&work->update_mtx);
	return (0);
}

static void
	main_window(t_work *work)
{
	setup_sighandlers();
	main_update_start(work);
	work_resume(work);
	win_create(&work->state->win,
		work->state->image->width, work->state->image->height);
	win_event_hook(&work->state->win, RT_WIN_EVENT_KEY_DOWN, rt_key_down, work);
	win_event_hook(&work->state->win, RT_WIN_EVENT_CLOSE, rt_exit, work);
	win_start(rt_loop, work);
}

void
	main_run(t_options *options, t_work *work)
{
	if (options->image_file == NULL)
		main_window(work);
	else
		main_image(work, options->image_file);
}

#else

void
	main_run(t_options *options, t_work *work)
{
	if (options->image_file == NULL)
	{
		ft_fprintf(STDERR_FILENO, "Cannot open window in joinc mode\n");
		exit(EXIT_FAILURE);
	}
	else
		main_image(work, options->image_file);
}

#endif
