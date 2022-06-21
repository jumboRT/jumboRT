#include "work.h"
#include "util.h"

#include <ft_printf.h>

#if defined RT_WORK_OPENCL

# ifdef RT_LINUX
#  include <CL/cl.h>
# else
#  include <OpenCL/cl.h>
# endif

# ifndef RT_WORK_OPENCL_CHUNK_SIZE
#  define RT_WORK_OPENCL_CHUNK_SIZE 4096
# endif

struct s_opencl_ctx {
	cl_context			context;
	cl_command_queue	command_queue;
	cl_program			program;
	cl_kernel			kernel;
	cl_mem				world_mem;
	cl_mem				ctx_mem;
	cl_mem				result_mem;
	cl_mem				primitives_mem;
	cl_mem				materials_mem;
	cl_mem				vertices_mem;
	t_context			ctx;
	t_result			result[RT_WORK_OPENCL_CHUNK_SIZE];
};

const static char *g_source_files[] = {
	"src/work/compute.c",
	"src/math/plane.c",
	"src/math/polynomial.c",
	"src/math/sphere.c",
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
	"src/math/sqrt.c",
	"src/math/sin.c",
	"src/math/cos.c",
	"src/math/tan.c",
	"src/world/intersect.c"
};

void
	*work_start(void *data)
{
	t_worker			*worker;
	uint64_t			begin;
	uint64_t			end;
	struct s_opencl_ctx	*cl_ctx;
	cl_int				status;
	size_t				global_work_size[1];
	size_t				local_work_size[1];

	worker = data;
	cl_ctx = worker->ctx;
	global_work_size[0] = RT_WORK_OPENCL_CHUNK_SIZE;
	local_work_size[0] = 64;
	while (work_sync(worker->work, &begin, &end, RT_WORK_OPENCL_CHUNK_SIZE))
	{
		status = clSetKernelArg(cl_ctx->kernel, 2, sizeof(begin), &begin);
		rt_assert(status == CL_SUCCESS, "clSetKernelArg 2 failed");
		status = clSetKernelArg(cl_ctx->kernel, 3, sizeof(end), &end);
		rt_assert(status == CL_SUCCESS, "clSetKernelArg 3 failed");
		status = clEnqueueNDRangeKernel(cl_ctx->command_queue, cl_ctx->kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
		rt_assert(status == CL_SUCCESS, "clEnqueueNDRangeKernel");
		status = clEnqueueReadBuffer(cl_ctx->command_queue, cl_ctx->result_mem, CL_TRUE, 0, sizeof(*cl_ctx->result) * RT_WORK_OPENCL_CHUNK_SIZE, cl_ctx->result, 0, NULL, NULL);
		rt_assert(status == CL_SUCCESS, "clEnqueueReadBuffer failed");
		queue_send(&worker->queue, cl_ctx->result, sizeof(*cl_ctx->result) * (end - begin));
	}
	return (NULL);
}

void
	work_create_buffers(t_work *work, struct s_opencl_ctx *cl_ctx)
{
	cl_int	status;

	cl_ctx->world_mem = clCreateBuffer(cl_ctx->context, CL_MEM_USE_HOST_PTR, sizeof(*work->state->world), work->state->world, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer world failed");
	cl_ctx->ctx_mem = clCreateBuffer(cl_ctx->context, CL_MEM_USE_HOST_PTR, sizeof(cl_ctx->ctx), &cl_ctx->ctx, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer ctx failed");
	cl_ctx->result_mem = clCreateBuffer(cl_ctx->context, CL_MEM_WRITE_ONLY, sizeof(*cl_ctx->result) * RT_WORK_OPENCL_CHUNK_SIZE, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer result failed");
	cl_ctx->primitives_mem = NULL;
	if (work->state->world->primitives_size != 0)
	{
		cl_ctx->primitives_mem = clCreateBuffer(cl_ctx->context, CL_MEM_USE_HOST_PTR, work->state->world->primitives_size, work->state->world->primitives, &status);
		rt_assert(status == CL_SUCCESS, "clCreateBuffer primitives failed");
	}
	cl_ctx->materials_mem = NULL;
	if (work->state->world->materials_size != 0)
	{
		cl_ctx->materials_mem = clCreateBuffer(cl_ctx->context, CL_MEM_USE_HOST_PTR, work->state->world->materials_size, work->state->world->materials, &status);
		rt_assert(status == CL_SUCCESS, "clCreateBuffer materials failed");
	}
	cl_ctx->vertices_mem = NULL;
	if (work->state->world->vertices_size != 0)
	{
		cl_ctx->vertices_mem = clCreateBuffer(cl_ctx->context, CL_MEM_USE_HOST_PTR, work->state->world->vertices_size, work->state->world->vertices, &status);
		rt_assert(status == CL_SUCCESS, "clCreateBuffer vertices failed");
	}
}

/* TODO: free mallocs */
void
	work_int_create(t_work *work)
{
	cl_context_properties	props[3];
	cl_platform_id			platform;
	cl_device_id			device;
	cl_int					status;
	struct s_opencl_ctx		*cl_ctx;
	char					**strings;
	size_t					*lengths;
	size_t					i;
	size_t					count;
	char					*error;
	char					*str;
	size_t					size;

	count = sizeof(g_source_files) / sizeof(*g_source_files);
	cl_ctx = rt_malloc(sizeof(*cl_ctx));
	strings = rt_malloc(sizeof(*strings) * count);
	lengths = rt_malloc(sizeof(*lengths) * count);
	
	i = 0;
	while (i < count)
	{
		strings[i] = rt_readfile(g_source_files[i], &error, &lengths[i]);
		rt_assert(strings[i] != NULL, error);
		i += 1;
	}

	status = clGetPlatformIDs(1, &platform, NULL);
	rt_assert(status == CL_SUCCESS, "clGetPlatformIDs failed");
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, NULL);
	rt_assert(status == CL_SUCCESS, "clGetDeviceIDs failed");
	props[0] = CL_CONTEXT_PLATFORM;
	props[1] = (cl_context_properties) platform;
	props[2] = 0;
	cl_ctx->context = clCreateContextFromType(props, CL_DEVICE_TYPE_DEFAULT, NULL, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateContextFromType failed");
	cl_ctx->command_queue = clCreateCommandQueueWithProperties(cl_ctx->context, device, 0, &status);
	rt_assert(status == CL_SUCCESS, "clCreateCommandQueue failed");
	work_create_buffers(work, cl_ctx);
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
	rt_assert(status == CL_SUCCESS, "clBuildProgram failed");
	cl_ctx->kernel = clCreateKernel(cl_ctx->program, "work_kernel", &status);
	rt_assert(status == CL_SUCCESS, "clCreateKernel failed");
	status = clSetKernelArg(cl_ctx->kernel, 0, sizeof(cl_mem), &cl_ctx->world_mem);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg 0 failed");
	status = clSetKernelArg(cl_ctx->kernel, 1, sizeof(cl_mem), &cl_ctx->ctx_mem);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg 1 failed");
	status = clSetKernelArg(cl_ctx->kernel, 4, sizeof(cl_mem), &cl_ctx->result_mem);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg 4 failed");
	work_add(work, work_start, cl_ctx);
}

void
	work_int_destroy(t_work *work)
{
	/* TODO */
	(void) work;
}

void
	work_int_resume(t_work *work)
{
	(void) work;
}

#endif

