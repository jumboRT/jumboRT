#include "cl_impl.h"
#include "util.h"

#if RT_USE_OPENCL

# define CHUNK_SIZE RT_WORK_OPENCL_CHUNK_SIZE

static void
	work_set_kernel_args(t_opencl_start_ctx *ctx,
			uint64_t begin, uint64_t end, int id)
{
	cl_int				status;

	status = clSetKernelArg(ctx->cl_ctx->work_kernel, 2, sizeof(begin), &begin);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg failed");
	status = clSetKernelArg(ctx->cl_ctx->work_kernel, 3, sizeof(end), &end);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg failed");
	status = clSetKernelArg(ctx->cl_ctx->work_kernel, 4, sizeof(cl_mem),
			&ctx->cl_ctx->result_mem[id]);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg failed");
	status = clSetKernelArg(ctx->cl_ctx->work_kernel, 5, sizeof(cl_mem),
			&ctx->cl_ctx->indices_mem[id]);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg failed");
	ctx->cb_ctx[id].worker = ctx->worker;
	ctx->cb_ctx[id].cl_ctx = ctx->cl_ctx;
	ctx->cb_ctx[id].id = id;
	ctx->cb_ctx[id].begin = begin;
	ctx->cb_ctx[id].end = end;
}

static void
	work_enqueue_kernel(t_opencl_start_ctx *ctx, int first, int id)
{
	static const size_t	global_work_size[1] = { RT_WORK_OPENCL_GLOBAL_SIZE };
	static const size_t	local_work_size[1] = { RT_WORK_OPENCL_LOCAL_SIZE };
	cl_int				status;
	cl_event			write_event;

	/* TODO: should these be in command queue 1?
	 * or even a third command queue? */
	if (first)
		status = clEnqueueWriteBuffer(ctx->cl_ctx->command_queue[0],
				ctx->cl_ctx->indices_mem[id], CL_FALSE, 0,
				sizeof(ctx->cl_ctx->indices[id]), &ctx->cl_ctx->indices[id], 0,
				NULL, &write_event);
	else
		status = clEnqueueWriteBuffer(ctx->cl_ctx->command_queue[0],
				ctx->cl_ctx->indices_mem[id], CL_FALSE, 0,
				sizeof(ctx->cl_ctx->indices[id]), &ctx->cl_ctx->indices[id], 1,
				&ctx->kernel_event[1 - id], &write_event);
	rt_assert(status == CL_SUCCESS, "clEnqueueWriteBuffer failed");
	status = clEnqueueNDRangeKernel(ctx->cl_ctx->command_queue[0],
			ctx->cl_ctx->work_kernel, 1, NULL, global_work_size,
			local_work_size, 1, &write_event, &ctx->kernel_event[id]);
	rt_assert(status == CL_SUCCESS, "clEnqueueNDRangeKernel failed");
	status = clReleaseEvent(write_event); /* TODO: check this */
	rt_assert(status == CL_SUCCESS, "clReleaseEvent failed");
}

static void
	work_enqueue_read(t_opencl_start_ctx *ctx, int id)
{
	cl_int		status;
	cl_event	read_event;

	status = clEnqueueReadBuffer(ctx->cl_ctx->command_queue[1],
			ctx->cl_ctx->result_mem[id], CL_FALSE, 0,
			sizeof(*ctx->cl_ctx->result) * RT_WORK_OPENCL_CHUNK_SIZE,
			ctx->cl_ctx->result, 1, &ctx->kernel_event[id], &read_event);
	rt_assert(status == CL_SUCCESS, "clEnqueueReadBuffer failed");
	status = clFlush(ctx->cl_ctx->command_queue[0]);
	rt_assert(status == CL_SUCCESS, "clFlush failed");
	status = clFlush(ctx->cl_ctx->command_queue[1]);
	rt_assert(status == CL_SUCCESS, "clFlush failed");
	ctx->cb_ctx[id].event = clCreateUserEvent(ctx->cl_ctx->context, &status);
	rt_assert(status == CL_SUCCESS, "clCreateuserEvent failed");
	status = clSetEventCallback(read_event, CL_COMPLETE, work_callback,
			&ctx->cb_ctx[id]);
	rt_assert(status == CL_SUCCESS, "clSetEventCallback failed");
	status = clReleaseEvent(read_event);
	rt_assert(status == CL_SUCCESS, "clReleaseEvent failed");
}

static void
	work_release_events(t_opencl_start_ctx *ctx, int id)
{
	cl_int	status;

	status = clReleaseEvent(ctx->kernel_event[id]);
	rt_assert(status == CL_SUCCESS, "clReleaseEvent failed");
	status = clWaitForEvents(1, &ctx->cb_ctx[id].event);
	rt_assert(status == CL_SUCCESS, "clWaitForEvents failed");
	status = clReleaseEvent(ctx->cb_ctx[id].event);
	rt_assert(status == CL_SUCCESS, "clReleaseEvent failed");
}

void
	*work_start(void *data)
{
	t_opencl_start_ctx	ctx;
	uint64_t			begin;
	uint64_t			end;
	int					id;

	ctx.worker = data;
	ctx.cl_ctx = ctx.worker->ctx;
	if (work_sync(ctx.worker->work, &begin, &end, CHUNK_SIZE))
	{
		id = 0;
		work_set_kernel_args(&ctx, begin, end, id);
		work_enqueue_kernel(&ctx, 1, id);
		work_enqueue_read(&ctx, id);
		while (work_sync(ctx.worker->work, &begin, &end, CHUNK_SIZE))
		{
			id = 1 - id;
			work_set_kernel_args(&ctx, begin, end, id);
			work_enqueue_kernel(&ctx, 1, id);
			work_release_events(&ctx, 1 - id);
			work_enqueue_read(&ctx, id);
		}
		work_release_events(&ctx, id);
	}
	return (NULL);
}

#endif

