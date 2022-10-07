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
	work_setup(void *data, cl_platform_id platform, cl_device_id device)
{
	t_work					*work;
	struct s_opencl_ctx		*cl_ctx;
	cl_int					status;
	size_t					i;
	t_opencl_program_ctx	program_ctx;

	work = data;
	cl_ctx = rt_malloc(sizeof(*cl_ctx));
	i = 0;
	while (i < RT_WORK_OPENCL_GLOBAL_SIZE)
	{
		ctx_init(&cl_ctx->ctx[i]);
		i += 1;
	}
	cl_ctx->indices[0] = 0;
	cl_ctx->indices[1] = 0;
	cl_ctx->context = work_create_context(platform);
	cl_ctx->command_queue[0] = work_create_queue(cl_ctx->context, device, 0);
	cl_ctx->command_queue[1] = work_create_queue(cl_ctx->context, device, 0);
	program_ctx.context = cl_ctx->context;
	program_ctx.platform = platform;
	program_ctx.device = device;
	opencl_load_program_path(&program_ctx, "kernel", 1);
	cl_ctx->program = program_ctx.program;
	cl_ctx->work_kernel = clCreateKernel(cl_ctx->program, "work_kernel", &status);
	rt_assert(status == CL_SUCCESS, "clCreateKernel work_kernel failed");
	work_null_buffers(cl_ctx);
	work_add(work, work_start, cl_ctx, RT_BACKEND_OPENCL);
}

void
	work_int_create_opencl(t_work *work)
{
	int	i;

	if (work->opts->cl_device_count == 0)
	{
		opencl_device_default(work_setup, work);
	}
	else
	{
		i = 0;
		while (i < work->opts->cl_device_count)
		{
			opencl_device_by_index(work->opts->cl_platforms[i],
					work->opts->cl_devices[i], work_setup, work);
			i += 1;
		}
	}
}

void
	work_int_destroy_opencl(t_work *work)
{
	size_t				i;
	size_t				j;
	struct s_opencl_ctx	*cl_ctx;
	cl_int				status;

	i = 0;
	while (i < work->count)
	{
		if (work->workers[i]->backend == RT_BACKEND_OPENCL)
		{
			cl_ctx = work->workers[i]->ctx;
			work_destroy_queue(cl_ctx->command_queue[0]);
			work_destroy_queue(cl_ctx->command_queue[1]);
			work_destroy_context(cl_ctx->context);
			status = clReleaseProgram(cl_ctx->program);
			rt_assert(status == CL_SUCCESS, "clReleaseProgram failed");
			status = clReleaseKernel(cl_ctx->work_kernel);
			rt_assert(status == CL_SUCCESS, "clReleaseKernel failed");
			work_release_buffers(cl_ctx);
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
	struct s_opencl_ctx *cl_ctx;

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

