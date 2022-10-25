/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   opencl.c                                       #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:09 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:09 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "work.h"
#include "util.h"
#include "cl_impl.h"

#include <libft.h>
#include <ft_printf.h>

#if RT_USE_OPENCL

void
	work_callback(cl_event event, cl_int event_command_status, void *user_data)
{
	struct s_opencl_callback_ctx	*cb_ctx;
	t_worker						*worker;
	struct s_opencl_ctx				*cl_ctx;
	cl_int							status;

	(void) event;
	(void) event_command_status;
	cb_ctx = user_data;
	worker = cb_ctx->worker;
	cl_ctx = cb_ctx->cl_ctx;
	work_send_results(worker, cl_ctx->result, cb_ctx->begin, cb_ctx->end);
	status = clSetUserEventStatus(cb_ctx->event, CL_COMPLETE);
	rt_assert(status == CL_SUCCESS, "clSetUserEventStatus failed");
}

static void
	work_int_destroy_opencl_int(struct s_opencl_ctx *cl_ctx)
{
	cl_int				status;

	work_destroy_queue(cl_ctx->command_queue[0]);
	work_destroy_queue(cl_ctx->command_queue[1]);
	work_destroy_context(cl_ctx->context);
	status = clReleaseProgram(cl_ctx->program);
	rt_assert(status == CL_SUCCESS, "clReleaseProgram failed");
	status = clReleaseKernel(cl_ctx->work_kernel);
	rt_assert(status == CL_SUCCESS, "clReleaseKernel failed");
	work_release_buffers(cl_ctx);
}

void
	work_int_destroy_opencl(t_work *work)
{
	size_t				i;
	size_t				j;
	struct s_opencl_ctx	*cl_ctx;

	i = 0;
	while (i < work->count)
	{
		if (work->workers[i]->backend == RT_BACKEND_OPENCL)
		{
			cl_ctx = work->workers[i]->ctx;
			work_int_destroy_opencl_int(cl_ctx);
			j = 0;
			while (j < RT_WORK_OPENCL_GLOBAL_SIZE)
			{
				ctx_destroy(&cl_ctx->ctx[j]);
				j += 1;
			}
			rt_free(cl_ctx);
		}
		i += 1;
	}
}

void
	work_int_resume_opencl(t_work *work)
{
	size_t				i;
	struct s_opencl_ctx	*cl_ctx;

	i = 0;
	while (i < work->count)
	{
		if (work->workers[i]->backend == RT_BACKEND_OPENCL)
		{
			cl_ctx = work->workers[i]->ctx;
			work_release_buffers(cl_ctx);
			work_create_buffers(work, cl_ctx);
		}
		i += 1;
	}
}

#endif
