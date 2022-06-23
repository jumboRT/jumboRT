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
#  define RT_WORK_OPENCL_GLOBAL_SIZE 1048576
# endif

# ifndef RT_WORK_OPENCL_LOCAL_SIZE
#  define RT_WORK_OPENCL_LOCAL_SIZE 256
# endif

struct s_opencl_ctx {
	cl_context			context;
	cl_command_queue	command_queue;
	cl_program			program;
	cl_kernel			work_kernel;
	cl_kernel			set_ptr_kernel;
	cl_mem				world_mem;
	cl_mem				ctx_mem;
	cl_mem				result_mem;
	cl_mem				primitives_mem;
	cl_mem				materials_mem;
	cl_mem				vertices_mem;
	cl_mem				accel_nodes_mem;
	cl_mem				accel_indices_mem;
	t_context			ctx;
	t_result			result[RT_WORK_OPENCL_GLOBAL_SIZE];
};

const static char *g_source_files[] = {
	"src/util/random.c",
	"src/work/compute.c",
	"src/math/plane.c",
	"src/math/polynomial.c",
	"src/math/sphere.c",
	"src/math/triangle.c",
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
	global_work_size[0] = RT_WORK_OPENCL_GLOBAL_SIZE;
	local_work_size[0] = RT_WORK_OPENCL_LOCAL_SIZE;
	while (work_sync(worker->work, &begin, &end, RT_WORK_OPENCL_GLOBAL_SIZE))
	{
		status = clSetKernelArg(cl_ctx->work_kernel, 2, sizeof(begin), &begin);
		rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 2 failed");
		status = clSetKernelArg(cl_ctx->work_kernel, 3, sizeof(end), &end);
		rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 3 failed");
		status = clEnqueueNDRangeKernel(cl_ctx->command_queue, cl_ctx->work_kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
		rt_assert(status == CL_SUCCESS, "clEnqueueNDRangeKernel work_kernel failed");
		status = clEnqueueReadBuffer(cl_ctx->command_queue, cl_ctx->result_mem, CL_TRUE, 0, sizeof(*cl_ctx->result) * RT_WORK_OPENCL_GLOBAL_SIZE, cl_ctx->result, 0, NULL, NULL);
		rt_assert(status == CL_SUCCESS, "clEnqueueReadBuffer failed");
		queue_send(&worker->queue, cl_ctx->result, sizeof(*cl_ctx->result) * (end - begin));
	}
	return (NULL);
}

void
	work_set_ptr(struct s_opencl_ctx *cl_ctx, cl_mem dst, uint64_t offset, cl_mem ptr)
{
	size_t	global_work_size[1];
	size_t	local_work_size[1];
	cl_int	status;

	global_work_size[0] = 1;
	local_work_size[0] = 1;
	status = clSetKernelArg(cl_ctx->set_ptr_kernel, 0, sizeof(cl_mem), &dst);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg set_ptr_kernel 0 failed");
	status = clSetKernelArg(cl_ctx->set_ptr_kernel, 1, sizeof(offset), &offset);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg set_ptr_kernel 1 failed");
	status = clSetKernelArg(cl_ctx->set_ptr_kernel, 2, sizeof(cl_mem), &ptr);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg set_ptr_kernel 2 failed");
	status = clEnqueueNDRangeKernel(cl_ctx->command_queue, cl_ctx->set_ptr_kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, NULL);
	rt_assert(status == CL_SUCCESS, "clEnqueueNDRangeKernel set_ptr_kernel failed");
}

cl_mem
	work_copy_array(struct s_opencl_ctx *cl_ctx, size_t size, void *ptr)
{
	cl_int	status;
	cl_mem	mem;

	if (size == 0)
		return (NULL);
	mem = clCreateBuffer(cl_ctx->context, CL_MEM_COPY_HOST_PTR, size, ptr, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer failed");
	return (mem);
}

void
	work_create_buffers(t_work *work, struct s_opencl_ctx *cl_ctx)
{
	cl_int	status;

	cl_ctx->world_mem = clCreateBuffer(cl_ctx->context, CL_MEM_COPY_HOST_PTR, sizeof(*work->state->world), work->state->world, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer world failed");
	cl_ctx->ctx_mem = clCreateBuffer(cl_ctx->context, CL_MEM_COPY_HOST_PTR, sizeof(cl_ctx->ctx), &cl_ctx->ctx, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer ctx failed");
	cl_ctx->result_mem = clCreateBuffer(cl_ctx->context, CL_MEM_WRITE_ONLY, sizeof(*cl_ctx->result) * RT_WORK_OPENCL_GLOBAL_SIZE, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer result failed");
	cl_ctx->primitives_mem = work_copy_array(cl_ctx, work->state->world->primitives_size, work->state->world->primitives);
	cl_ctx->vertices_mem = work_copy_array(cl_ctx, work->state->world->vertices_size, work->state->world->vertices);
	cl_ctx->materials_mem = work_copy_array(cl_ctx, work->state->world->materials_size, work->state->world->materials);
	cl_ctx->accel_nodes_mem = work_copy_array(cl_ctx, work->state->world->accel_nodes_size, work->state->world->accel_nodes);
	cl_ctx->accel_indices_mem = work_copy_array(cl_ctx, work->state->world->accel_indices_size, work->state->world->accel_indices);
	work_set_ptr(cl_ctx, cl_ctx->world_mem, offsetof(t_world, primitives), cl_ctx->primitives_mem);
	work_set_ptr(cl_ctx, cl_ctx->world_mem, offsetof(t_world, materials), cl_ctx->materials_mem);
	work_set_ptr(cl_ctx, cl_ctx->world_mem, offsetof(t_world, vertices), cl_ctx->vertices_mem);
	work_set_ptr(cl_ctx, cl_ctx->world_mem, offsetof(t_world, accel_nodes), cl_ctx->accel_nodes_mem);
	work_set_ptr(cl_ctx, cl_ctx->world_mem, offsetof(t_world, accel_indices), cl_ctx->accel_indices_mem);
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
}

void
	work_setup(t_work *work, cl_platform_id platform, cl_device_id device)
{
	struct s_opencl_ctx		*cl_ctx;
	cl_context_properties	props[3];
	cl_int					status;

	cl_ctx = rt_malloc(sizeof(*cl_ctx));
	ctx_init(&cl_ctx->ctx);
	props[0] = CL_CONTEXT_PLATFORM;
	props[1] = (cl_context_properties) platform;
	props[2] = 0;
	cl_ctx->context = clCreateContextFromType(props, CL_DEVICE_TYPE_DEFAULT, NULL, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateContextFromType failed");
	cl_ctx->command_queue = clCreateCommandQueueWithProperties(cl_ctx->context, device, 0, &status);
	rt_assert(status == CL_SUCCESS, "clCreateCommandQueue failed");
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
	status = clSetKernelArg(cl_ctx->work_kernel, 4, sizeof(cl_mem), &cl_ctx->result_mem);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 4 failed");
	work_add(work, work_start, cl_ctx);
}

/* TODO: free mallocs */
void
	work_int_create(t_work *work)
{
	cl_platform_id			platform;
	cl_device_id			device;
	cl_int					status;

	status = clGetPlatformIDs(1, &platform, NULL);
	rt_assert(status == CL_SUCCESS, "clGetPlatformIDs failed");
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, NULL);
	rt_assert(status == CL_SUCCESS, "clGetDeviceIDs failed");
	work_setup(work, platform, device);
	work_setup(work, platform, device);
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

