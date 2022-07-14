#include "work.h"
#include "util.h"

#include <ft_printf.h>

#if defined RT_WORK_OPENCL

# ifdef RT_LINUX
#  include <CL/cl.h>
# else
#  include <OpenCL/cl.h>
# endif

# ifndef RT_WORK_OPENCL_GLOBAL_SIZE
#  define RT_WORK_OPENCL_GLOBAL_SIZE (1ULL << 20)
# endif

# ifndef RT_WORK_OPENCL_LOCAL_SIZE
#  define RT_WORK_OPENCL_LOCAL_SIZE 32
# endif

struct s_opencl_ctx {
	cl_context			context;
	cl_command_queue	command_queue[2];
	cl_program			program;
	cl_kernel			work_kernel;
	cl_kernel			set_ptr_kernel;
	cl_mem				world_mem;
	cl_mem				ctx_mem;
	cl_mem				result_mem[2];
	cl_mem				primitives_mem;
	cl_mem				materials_mem;
	cl_mem				vertices_mem;
	cl_mem				accel_nodes_mem;
	cl_mem				accel_indices_mem;
	cl_mem				accel_degenerates_mem;
	t_context			ctx[RT_WORK_OPENCL_GLOBAL_SIZE];
	t_result			result[RT_WORK_OPENCL_GLOBAL_SIZE];
	int					id;
};

struct s_opencl_callback_ctx {
	t_worker			*worker;
	struct s_opencl_ctx	*cl_ctx;
	int					id;
	cl_event			event;
	size_t				size;
};

const static char *g_source_files[] = {
	"src/util/random.c",
	"src/work/compute.c",
	"src/math/plane.c",
	"src/math/polynomial.c",
	"src/math/sphere.c",
	"src/math/triangle.c",
	"src/math/cylinder.c",
	"src/math/cone.c",
	"src/math/ray_constr.c",
	"src/math/vec_arith.c",
	"src/math/vec_arith_fast.c",
	"src/math/vec_constr.c",
	"src/math/vec_constr_fast.c",
	"src/math/vec_geo.c",
	"src/math/vec_geo_fast.c",
	"src/math/vec_get.c",
	"src/math/vec_get_fast.c",
	"src/math/vec_size.c",
	"src/math/vec_size_fast.c",
	"src/math/vec_rotate.c",
	"src/math/vec_set.c",
	"src/math/sqrt.c",
	"src/math/sin.c",
	"src/math/cos.c",
	"src/math/tan.c",
	"src/math/min.c",
	"src/math/max.c",
	"src/math/abs.c",
	"src/math/pow.c",
	"src/world/intersect.c",
	"src/world/intersect_prim.c",
	"src/world/primitive.c",
	"src/world/node.c",
	"src/world/common.c",
	"src/world/trace.c"
};

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
	queue_send(&worker->queue, cl_ctx->result, sizeof(*cl_ctx->result) * cb_ctx->size);
	status = clSetUserEventStatus(cb_ctx->event, CL_COMPLETE);
	rt_assert(status == CL_SUCCESS, "clSetUserEventStatus work_callback failed");
}

void
	*work_start(void *data)
{
	t_worker						*worker;
	uint64_t						begin;
	uint64_t						end;
	struct s_opencl_ctx				*cl_ctx;
	cl_int							status;
	size_t							global_work_size[1];
	size_t							local_work_size[1];
	cl_event						kernel_event[2];
	cl_event						read_event;
	int								id;
	struct s_opencl_callback_ctx	cb_ctx[2];

	worker = data;
	cl_ctx = worker->ctx;
	global_work_size[0] = RT_WORK_OPENCL_GLOBAL_SIZE;
	local_work_size[0] = RT_WORK_OPENCL_LOCAL_SIZE;
	cb_ctx[0].worker = worker;
	cb_ctx[0].cl_ctx = cl_ctx;
	cb_ctx[0].id = 0;
	cb_ctx[1].worker = worker;
	cb_ctx[1].cl_ctx = cl_ctx;
	cb_ctx[1].id = 1;
	if (work_sync(worker->work, &begin, &end, RT_WORK_OPENCL_GLOBAL_SIZE))
	{
		id = 0;
		status = clSetKernelArg(cl_ctx->work_kernel, 2, sizeof(begin), &begin);
		rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 2 failed");
		status = clSetKernelArg(cl_ctx->work_kernel, 3, sizeof(end), &end);
		rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 3 failed");
		status = clSetKernelArg(cl_ctx->work_kernel, 4, sizeof(cl_mem), &cl_ctx->result_mem[id]);
		rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 4 failed");
		cb_ctx[id].size = end - begin;
		status = clEnqueueNDRangeKernel(cl_ctx->command_queue[0], cl_ctx->work_kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, &kernel_event[id]);
		rt_assert(status == CL_SUCCESS, "clEnqueueNDRangeKernel work_kernel failed");
		status = clEnqueueReadBuffer(cl_ctx->command_queue[1], cl_ctx->result_mem[id], CL_FALSE, 0, sizeof(*cl_ctx->result) * RT_WORK_OPENCL_GLOBAL_SIZE, cl_ctx->result, 1, &kernel_event[id], &read_event);
		rt_assert(status == CL_SUCCESS, "clEnqueueReadBuffer work_kernel failed");
		cb_ctx[id].event = clCreateUserEvent(cl_ctx->context, &status);
		rt_assert(status == CL_SUCCESS, "clCreateUserEvent work_kernel failed");
		status = clSetEventCallback(read_event, CL_COMPLETE, work_callback, &cb_ctx[id]);
		rt_assert(status == CL_SUCCESS, "clSetEventCallback work_kernel failed");
		status = clReleaseEvent(read_event);
		rt_assert(status == CL_SUCCESS, "clReleaseEvent work_kernel failed");
		while (work_sync(worker->work, &begin, &end, RT_WORK_OPENCL_GLOBAL_SIZE))
		{
			id = 1 - id;
			status = clSetKernelArg(cl_ctx->work_kernel, 2, sizeof(begin), &begin);
			rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 2 failed");
			status = clSetKernelArg(cl_ctx->work_kernel, 3, sizeof(end), &end);
			rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 3 failed");
			status = clSetKernelArg(cl_ctx->work_kernel, 4, sizeof(cl_mem), &cl_ctx->result_mem[id]);
			rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 4 failed");
			cb_ctx[id].size = end - begin;
			status = clEnqueueNDRangeKernel(cl_ctx->command_queue[0], cl_ctx->work_kernel, 1, NULL, global_work_size, local_work_size, 1, &kernel_event[1 - id], &kernel_event[id]);
			rt_assert(status == CL_SUCCESS, "clEnqueueNDRangeKernel work_kernel failed");
			status = clReleaseEvent(kernel_event[1 - id]);
			rt_assert(status == CL_SUCCESS, "clReleaseEvent work_kernel failed");
			status = clWaitForEvents(1, &cb_ctx[1 - id].event);
			rt_assert(status == CL_SUCCESS, "clWaitForEvents work_kernel failed");
			status = clReleaseEvent(cb_ctx[1 - id].event);
			rt_assert(status == CL_SUCCESS, "clReleaseEvent work_kernel failed");
			status = clEnqueueReadBuffer(cl_ctx->command_queue[1], cl_ctx->result_mem[id], CL_FALSE, 0, sizeof(*cl_ctx->result) * RT_WORK_OPENCL_GLOBAL_SIZE, cl_ctx->result, 1, &kernel_event[id], &read_event);
			rt_assert(status == CL_SUCCESS, "clEnqueueReadBuffer work_kernel failed");
			cb_ctx[id].event = clCreateUserEvent(cl_ctx->context, &status);
			rt_assert(status == CL_SUCCESS, "clCreateUserEvent work_kernel failed");
			status = clSetEventCallback(read_event, CL_COMPLETE, work_callback, &cb_ctx[id]);
			rt_assert(status == CL_SUCCESS, "clSetEventCallback work_kernel failed");
			status = clReleaseEvent(read_event);
			rt_assert(status == CL_SUCCESS, "clReleaseEvent work_kernel failed");
		}
		status = clReleaseEvent(kernel_event[id]);
		rt_assert(status == CL_SUCCESS, "clReleaseEvent work_kernel failed");
		status = clReleaseEvent(cb_ctx[id].event);
		rt_assert(status == CL_SUCCESS, "clReleaseEvent work_kernel failed");
	}
	return (NULL);
}

void
	work_set_ptr(struct s_opencl_ctx *cl_ctx, cl_mem dst, cl_int offset, cl_mem ptr)
{
	size_t		global_work_size[1];
	size_t		local_work_size[1];
	cl_int		status;
	cl_event	event;

	global_work_size[0] = 1;
	local_work_size[0] = 1;
	status = clSetKernelArg(cl_ctx->set_ptr_kernel, 0, sizeof(cl_mem), &dst);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg set_ptr_kernel 0 failed");
	status = clSetKernelArg(cl_ctx->set_ptr_kernel, 1, sizeof(offset), &offset);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg set_ptr_kernel 1 failed");
	status = clSetKernelArg(cl_ctx->set_ptr_kernel, 2, sizeof(cl_mem), &ptr);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg set_ptr_kernel 2 failed");
	status = clEnqueueNDRangeKernel(cl_ctx->command_queue[0], cl_ctx->set_ptr_kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, &event);
	rt_assert(status == CL_SUCCESS, "clEnqueueNDRangeKernel set_ptr_kernel failed");
	status = clWaitForEvents(1, &event);
	rt_assert(status == CL_SUCCESS, "clWaitForEvents set_ptr_kernel failed");
	status = clReleaseEvent(event);
	rt_assert(status == CL_SUCCESS, "clReleaseEvent set_ptr_kernel failed");
}

cl_mem
	work_copy_array(struct s_opencl_ctx *cl_ctx, size_t size, void *ptr)
{
	cl_int	status;
	cl_mem	mem;

	if (size == 0)
		return (NULL);
	mem = clCreateBuffer(cl_ctx->context, CL_MEM_READ_ONLY, size, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer failed");
	status = clEnqueueWriteBuffer(cl_ctx->command_queue[0], mem, CL_TRUE, 0, size, ptr, 0, NULL, NULL);
	rt_assert(status == CL_SUCCESS, "clEnqueueWriteBuffer failed");
	return (mem);
}

void
	work_create_buffers(t_work *work, struct s_opencl_ctx *cl_ctx)
{
	cl_int	status;

	cl_ctx->world_mem = work_copy_array(cl_ctx, sizeof(*work->state->world), work->state->world);
	cl_ctx->ctx_mem = work_copy_array(cl_ctx, sizeof(*cl_ctx->ctx) * RT_WORK_OPENCL_GLOBAL_SIZE, cl_ctx->ctx);
	cl_ctx->result_mem[0] = clCreateBuffer(cl_ctx->context, CL_MEM_WRITE_ONLY, sizeof(*cl_ctx->result) * RT_WORK_OPENCL_GLOBAL_SIZE, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer result[0] failed");
	cl_ctx->result_mem[1] = clCreateBuffer(cl_ctx->context, CL_MEM_WRITE_ONLY, sizeof(*cl_ctx->result) * RT_WORK_OPENCL_GLOBAL_SIZE, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer result[1] failed");
	cl_ctx->primitives_mem = work_copy_array(cl_ctx, work->state->world->primitives_size, work->state->world->primitives);
	cl_ctx->vertices_mem = work_copy_array(cl_ctx, work->state->world->vertices_size, work->state->world->vertices);
	cl_ctx->materials_mem = work_copy_array(cl_ctx, work->state->world->materials_size, work->state->world->materials);
	cl_ctx->accel_nodes_mem = work_copy_array(cl_ctx, work->state->world->accel_nodes_size, work->state->world->accel_nodes);
	cl_ctx->accel_indices_mem = work_copy_array(cl_ctx, work->state->world->accel_indices_size, work->state->world->accel_indices);
	cl_ctx->accel_degenerates_mem = work_copy_array(cl_ctx, work->state->world->accel_degenerates_size, work->state->world->accel_degenerates);
	work_set_ptr(cl_ctx, cl_ctx->world_mem, 0, cl_ctx->primitives_mem);
	work_set_ptr(cl_ctx, cl_ctx->world_mem, 1, cl_ctx->materials_mem);
	work_set_ptr(cl_ctx, cl_ctx->world_mem, 2, cl_ctx->vertices_mem);
	work_set_ptr(cl_ctx, cl_ctx->world_mem, 3, cl_ctx->accel_nodes_mem);
	work_set_ptr(cl_ctx, cl_ctx->world_mem, 4, cl_ctx->accel_indices_mem);
	work_set_ptr(cl_ctx, cl_ctx->world_mem, 5, cl_ctx->accel_degenerates_mem);
}

void
	work_create_program(struct s_opencl_ctx *cl_ctx, cl_device_id device)
{
	char	**strings;
	size_t	*lengths;
	size_t	i;
	cl_int	status;
	char	*error;
	size_t	count;
	char	*str;
	size_t	size;

	count = sizeof(g_source_files) / sizeof(*g_source_files);
	strings = rt_malloc(sizeof(*strings) * count);
	lengths = rt_malloc(sizeof(*lengths) * count);
	i = 0;
	while (i < count)
	{
		strings[i] = rt_readfile(g_source_files[i], &error, &lengths[i]);
		rt_assert(strings[i] != NULL, error);
		i += 1;
	}
	cl_ctx->program = clCreateProgramWithSource(cl_ctx->context, count, (const char**) strings, lengths, &status);
	rt_assert(status == CL_SUCCESS, "clCreateProgramWithSource failed");
	status = clBuildProgram(cl_ctx->program, 1, &device, "-I include -D RT_OPENCL -D GLOBAL=__global", NULL, NULL);
	if (status != CL_SUCCESS)
	{
		status = clGetProgramBuildInfo(cl_ctx->program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &size);
		rt_assert(status == CL_SUCCESS, "clGetProgramBuildInfo failed");
		str = rt_malloc(size);
		status = clGetProgramBuildInfo(cl_ctx->program, device, CL_PROGRAM_BUILD_LOG, size, str, NULL);
		rt_assert(status == CL_SUCCESS, "clGetProgramBuildInfo failed");
		ft_printf("%s\n", str);
		rt_assert(0, "clBuildProgram failed");
	}
	i = 0;
	while (i < count)
	{
		rt_free(strings[i]);
		i += 1;
	}
	rt_free(strings);
	rt_free(lengths);
}

#ifdef RT_LINUX

cl_command_queue
	work_create_queue(cl_context context, cl_device_id device, cl_command_queue_properties props)
{
	cl_int				status;
	cl_command_queue	result;
	cl_queue_properties	queue_props[3];

	queue_props[0] = CL_QUEUE_PROPERTIES;
	queue_props[1] = props;
	queue_props[2] = 0;
	result = clCreateCommandQueueWithProperties(context, device, queue_props, &status);
	rt_assert(status == CL_SUCCESS, "clCreateCommandQueue failed");
	return (result);
}

#else

cl_command_queue
	work_create_queue(cl_context context, cl_device_id device, cl_command_queue_properties props)
{
	cl_int				status;
	cl_command_queue	result;

	result = clCreateCommandQueue(context, device, 0, &status);
	rt_assert(status == CL_SUCCESS, "clCreateCommandQueue failed");
	return (result);
}

#endif

void
	work_setup(t_work *work, cl_platform_id platform, cl_device_id device)
{
	struct s_opencl_ctx		*cl_ctx;
	cl_context_properties	context_props[3];
	cl_int					status;
	size_t					i;

	cl_ctx = rt_malloc(sizeof(*cl_ctx));
	i = 0;
	while (i < RT_WORK_OPENCL_GLOBAL_SIZE)
	{
		ctx_init(&cl_ctx->ctx[i]);
		i += 1;
	}
	context_props[0] = CL_CONTEXT_PLATFORM;
	context_props[1] = (cl_context_properties) platform;
	context_props[2] = 0;
	cl_ctx->context = clCreateContextFromType(context_props, CL_DEVICE_TYPE_DEFAULT, NULL, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateContextFromType failed");
	cl_ctx->command_queue[0] = work_create_queue(cl_ctx->context, device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);
	cl_ctx->command_queue[1] = work_create_queue(cl_ctx->context, device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);
	work_create_program(cl_ctx, device);
	cl_ctx->work_kernel = clCreateKernel(cl_ctx->program, "work_kernel", &status);
	rt_assert(status == CL_SUCCESS, "clCreateKernel work_kernel failed");
	cl_ctx->set_ptr_kernel = clCreateKernel(cl_ctx->program, "set_ptr_kernel", &status);
	work_create_buffers(work, cl_ctx);
	rt_assert(status == CL_SUCCESS, "clCreateKernel set_ptr_kernel failed");
	status = clSetKernelArg(cl_ctx->work_kernel, 0, sizeof(cl_mem), &cl_ctx->world_mem);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 0 failed");
	status = clSetKernelArg(cl_ctx->work_kernel, 1, sizeof(cl_mem), &cl_ctx->ctx_mem);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 1 failed");
	work_add(work, work_start, cl_ctx);
}

void
	work_int_create(t_work *work)
{
	cl_platform_id			platform;
	cl_device_id			device;
	cl_int					status;

	printf("doing first opencl call\n");
	status = clGetPlatformIDs(1, &platform, NULL);
	printf("got past first opencl call\n");
	rt_assert(status == CL_SUCCESS, "clGetPlatformIDs failed");
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, NULL);
	rt_assert(status == CL_SUCCESS, "clGetDeviceIDs failed");
	work_setup(work, platform, device);
}

void
	work_int_destroy(t_work *work)
{
	size_t				i;
	size_t				j;
	struct s_opencl_ctx	*cl_ctx;
	cl_int				status;

	i = 0;
	while (i < work->count)
	{
		cl_ctx = work->workers[i]->ctx;
		status = clReleaseContext(cl_ctx->context);
		rt_assert(status == CL_SUCCESS, "clReleaseContext failed");
		status = clReleaseCommandQueue(cl_ctx->command_queue[0]);
		rt_assert(status == CL_SUCCESS, "clReleaseCommandQueue failed");
		status = clReleaseCommandQueue(cl_ctx->command_queue[1]);
		rt_assert(status == CL_SUCCESS, "clReleaseCommandQueue failed");
		status = clReleaseProgram(cl_ctx->program);
		rt_assert(status == CL_SUCCESS, "clReleaseProgram failed");
		status = clReleaseKernel(cl_ctx->work_kernel);
		rt_assert(status == CL_SUCCESS, "clReleaseKernel failed");
		status = clReleaseKernel(cl_ctx->set_ptr_kernel);
		rt_assert(status == CL_SUCCESS, "clReleaseKernel failed");
		status = clReleaseMemObject(cl_ctx->world_mem);
		rt_assert(status == CL_SUCCESS, "clReleaseMemObject failed");
		status = clReleaseMemObject(cl_ctx->ctx_mem);
		rt_assert(status == CL_SUCCESS, "clReleaseMemObject failed");
		status = clReleaseMemObject(cl_ctx->result_mem[0]);
		rt_assert(status == CL_SUCCESS, "clReleaseMemObject failed");
		status = clReleaseMemObject(cl_ctx->result_mem[1]);
		rt_assert(status == CL_SUCCESS, "clReleaseMemObject failed");
		status = clReleaseMemObject(cl_ctx->primitives_mem);
		rt_assert(status == CL_SUCCESS, "clReleaseMemObject failed");
		status = clReleaseMemObject(cl_ctx->materials_mem);
		rt_assert(status == CL_SUCCESS, "clReleaseMemObject failed");
		status = clReleaseMemObject(cl_ctx->vertices_mem);
		rt_assert(status == CL_SUCCESS, "clReleaseMemObject failed");
		status = clReleaseMemObject(cl_ctx->accel_nodes_mem);
		rt_assert(status == CL_SUCCESS, "clReleaseMemObject failed");
		status = clReleaseMemObject(cl_ctx->accel_indices_mem);
		rt_assert(status == CL_SUCCESS, "clReleaseMemObject failed");
		status = clReleaseMemObject(cl_ctx->accel_degenerates_mem);
		rt_assert(status == CL_SUCCESS, "clReleaseMemObject failed");
		j = 0;
		while (j < RT_WORK_OPENCL_GLOBAL_SIZE)
		{
			ctx_destroy(&cl_ctx->ctx[j]);
			j += 1;
		}
		i += 1;
	}
	rt_free(cl_ctx);
}

void
	work_int_resume(t_work *work)
{
	(void) work;
}

#endif

