#include "work.h"
#include "util.h"
#include "cl_impl.h"

#include <libft.h>
#include <ft_printf.h>

#if RT_USE_OPENCL

static void
	work_setup_int(struct s_opencl_ctx *cl_ctx,
			cl_platform_id platform, cl_device_id device)
{
	cl_ctx->indices[0] = 0;
	cl_ctx->indices[1] = 0;
	cl_ctx->context = work_create_context(platform);
	cl_ctx->command_queue[0] = work_create_queue(cl_ctx->context, device, 0);
	cl_ctx->command_queue[1] = work_create_queue(cl_ctx->context, device, 0);
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
	work_setup_int(cl_ctx, platform, device);
	program_ctx.context = cl_ctx->context;
	program_ctx.platform = platform;
	program_ctx.device = device;
	opencl_load_program_path(&program_ctx, "kernel", 1);
	cl_ctx->program = program_ctx.program;
	cl_ctx->work_kernel = clCreateKernel(cl_ctx->program,
			"work_kernel", &status);
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

#endif
