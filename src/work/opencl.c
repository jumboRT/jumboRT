#include "work.h"
#include "util.h"

#include <libft.h>
#include <ft_printf.h>

#ifndef RT_USE_OPENCL
# define RT_USE_OPENCL 1
#endif

#if RT_USE_OPENCL

# if defined RT_LINUX
#  include <CL/cl.h>
# elif defined RT_WINDOWS
#  include <CL/cl.h>
# else
#  include <OpenCL/cl.h>
# endif

# ifndef RT_WORK_OPENCL_GLOBAL_SIZE
#  if defined RT_MACOS
#   define RT_WORK_OPENCL_GLOBAL_SIZE (1ULL << 10)
#  else
#   define RT_WORK_OPENCL_GLOBAL_SIZE (1ULL << 16)
#  endif
# endif

# ifndef RT_WORK_OPENCL_LOCAL_SIZE
#  if defined RT_MACOS
#   define RT_WORK_OPENCL_LOCAL_SIZE (1ULL << 6)
#  else
#   define RT_WORK_OPENCL_LOCAL_SIZE (1ULL << 5)
#  endif
# endif

# ifndef RT_WORK_OPENCL_CHUNK_SIZE
#  if defined RT_MACOS
#   define RT_WORK_OPENCL_CHUNK_SIZE (1ULL << 10)
#  else
#   define RT_WORK_OPENCL_CHUNK_SIZE (1ULL << 18)
#  endif
# endif

struct s_opencl_ctx {
	cl_context			context;
	cl_command_queue	command_queue[2];
	cl_program			program;
	cl_kernel			work_kernel;
	cl_mem				world_mem;
	cl_mem				ctx_mem;
	cl_mem				result_mem[2];
	cl_mem				indices_mem[2];
	cl_mem				primitives_mem;
	cl_mem				materials_mem;
	cl_mem				vertices_mem;
	cl_mem				accel_nodes_mem;
	cl_mem				accel_indices_mem;
	cl_mem				accel_degenerates_mem;
	cl_mem				texture_data_mem;
	cl_mem				textures_mem;
	cl_mem				bxdfs_mem;
	cl_mem				lights_mem;
	t_context			ctx[RT_WORK_OPENCL_GLOBAL_SIZE];
	t_result			result[RT_WORK_OPENCL_CHUNK_SIZE];
	cl_uint				indices[2];
	int					id;
};

struct s_opencl_callback_ctx {
	t_worker			*worker;
	struct s_opencl_ctx	*cl_ctx;
	int					id;
	cl_event			event;
	uint64_t			begin;
	uint64_t			end;
};

static char
	*platform_name(cl_platform_id platform)
{
	cl_int	status;
	char	*name;
	size_t	size;

	status = clGetPlatformInfo(platform, CL_PLATFORM_NAME, 0, NULL, &size);
	rt_assert(status == CL_SUCCESS, "clGetPlatformInfo failed");
	name = rt_malloc(size + 1);
	status = clGetPlatformInfo(platform, CL_PLATFORM_NAME, size, name, NULL);
	rt_assert(status == CL_SUCCESS, "clGetPlatformInfo failed");
	name[size] = '\0';
	return (name);
}

static char
	*device_name(cl_device_id device)
{
	cl_int	status;
	char	*name;
	size_t	size;

	status = clGetDeviceInfo(device, CL_DEVICE_NAME, 0, NULL, &size);
	rt_assert(status == CL_SUCCESS, "clGetDeviceInfo failed");
	name = rt_malloc(size + 1);
	status = clGetDeviceInfo(device, CL_DEVICE_NAME, size, name, NULL);
	rt_assert(status == CL_SUCCESS, "clGetDeviceInfo failed");
	name[size] = '\0';
	return (name);
}

static char
	*clean_name(char *name)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (name[i] != '\0')
	{
		if (ft_isalnum(name[i]))
			name[j++] = name[i];
		i++;
	}
	name[j] = '\0';
	return (name);
}

static char
	*device_file(cl_platform_id platform, cl_device_id device)
{
	size_t	size;
	char	*pname;
	char	*dname;
	char	*file;

	pname = clean_name(platform_name(platform));
	dname = clean_name(device_name(device));
	size = ft_strlen(pname) + ft_strlen(dname) + 13;
	file = rt_malloc(size);
	file[0] = '\0';
	ft_strlcat(file, "kernel-", size);
	ft_strlcat(file, pname, size);
	ft_strlcat(file, "-", size);
	ft_strlcat(file, dname, size);
	ft_strlcat(file, ".bin", size);
	rt_free(pname);
	rt_free(dname);
	return (file);
}

static void
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
	rt_assert(status == CL_SUCCESS, "clSetUserEventStatus work_callback failed");
}

static void
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
	cl_event						write_event;
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
	if (work_sync(worker->work, &begin, &end, RT_WORK_OPENCL_CHUNK_SIZE))
	{
		id = 0;
		status = clSetKernelArg(cl_ctx->work_kernel, 2, sizeof(begin), &begin);
		rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 2 failed");
		status = clSetKernelArg(cl_ctx->work_kernel, 3, sizeof(end), &end);
		rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 3 failed");
		status = clSetKernelArg(cl_ctx->work_kernel, 4, sizeof(cl_mem), &cl_ctx->result_mem[id]);
		rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 4 failed");
		status = clSetKernelArg(cl_ctx->work_kernel, 5, sizeof(cl_mem), &cl_ctx->indices_mem[id]);
		rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 5 failed");
		cb_ctx[id].begin = begin;
		cb_ctx[id].end = end;
		status = clEnqueueWriteBuffer(cl_ctx->command_queue[0], cl_ctx->indices_mem[id], CL_FALSE, 0, sizeof(cl_ctx->indices[id]), &cl_ctx->indices[id], 0, NULL, &write_event);
		rt_assert(status == CL_SUCCESS, "clEnqueueWriteBuffer work_kernel failed");
		status = clEnqueueNDRangeKernel(cl_ctx->command_queue[0], cl_ctx->work_kernel, 1, NULL, global_work_size, local_work_size, 1, &write_event, &kernel_event[id]);
		rt_assert(status == CL_SUCCESS, "clEnqueueNDRangeKernel work_kernel failed");
		status = clEnqueueReadBuffer(cl_ctx->command_queue[1], cl_ctx->result_mem[id], CL_FALSE, 0, sizeof(*cl_ctx->result) * RT_WORK_OPENCL_CHUNK_SIZE, cl_ctx->result, 1, &kernel_event[id], &read_event);
		rt_assert(status == CL_SUCCESS, "clEnqueueReadBuffer work_kernel failed");
		status = clFlush(cl_ctx->command_queue[0]);
		rt_assert(status == CL_SUCCESS, "clFlush work_kernel failed");
		status = clFlush(cl_ctx->command_queue[1]);
		rt_assert(status == CL_SUCCESS, "clFlush work_kernel failed");
		cb_ctx[id].event = clCreateUserEvent(cl_ctx->context, &status);
		rt_assert(status == CL_SUCCESS, "clCreateUserEvent work_kernel failed");
		status = clSetEventCallback(read_event, CL_COMPLETE, work_callback, &cb_ctx[id]);
		rt_assert(status == CL_SUCCESS, "clSetEventCallback work_kernel failed");
		status = clReleaseEvent(read_event);
		rt_assert(status == CL_SUCCESS, "clReleaseEvent work_kernel failed");
		while (work_sync(worker->work, &begin, &end, RT_WORK_OPENCL_CHUNK_SIZE))
		{
			id = 1 - id;
			status = clSetKernelArg(cl_ctx->work_kernel, 2, sizeof(begin), &begin);
			rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 2 failed");
			status = clSetKernelArg(cl_ctx->work_kernel, 3, sizeof(end), &end);
			rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 3 failed");
			status = clSetKernelArg(cl_ctx->work_kernel, 4, sizeof(cl_mem), &cl_ctx->result_mem[id]);
			rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 4 failed");
			status = clSetKernelArg(cl_ctx->work_kernel, 5, sizeof(cl_mem), &cl_ctx->indices_mem[id]);
			rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 5 failed");
			cb_ctx[id].begin = begin;
			cb_ctx[id].end = end;
			status = clEnqueueWriteBuffer(cl_ctx->command_queue[0], cl_ctx->indices_mem[id], CL_FALSE, 0, sizeof(cl_ctx->indices[id]), &cl_ctx->indices[id], 1, &kernel_event[1 - id], &write_event);
			rt_assert(status == CL_SUCCESS, "clEnqueueWriteBuffer work_kernel failed");
			status = clEnqueueNDRangeKernel(cl_ctx->command_queue[0], cl_ctx->work_kernel, 1, NULL, global_work_size, local_work_size, 1, &write_event, &kernel_event[id]);
			rt_assert(status == CL_SUCCESS, "clEnqueueNDRangeKernel work_kernel failed");
			status = clReleaseEvent(kernel_event[1 - id]);
			rt_assert(status == CL_SUCCESS, "clReleaseEvent work_kernel failed");
			status = clWaitForEvents(1, &cb_ctx[1 - id].event);
			rt_assert(status == CL_SUCCESS, "clWaitForEvents work_kernel failed");
			status = clReleaseEvent(cb_ctx[1 - id].event);
			rt_assert(status == CL_SUCCESS, "clReleaseEvent work_kernel failed");
			status = clEnqueueReadBuffer(cl_ctx->command_queue[1], cl_ctx->result_mem[id], CL_FALSE, 0, sizeof(*cl_ctx->result) * RT_WORK_OPENCL_CHUNK_SIZE, cl_ctx->result, 1, &kernel_event[id], &read_event);
			rt_assert(status == CL_SUCCESS, "clEnqueueReadBuffer work_kernel failed");
			status = clFlush(cl_ctx->command_queue[0]);
			rt_assert(status == CL_SUCCESS, "clFlush work_kernel failed");
			status = clFlush(cl_ctx->command_queue[1]);
			rt_assert(status == CL_SUCCESS, "clFlush work_kernel failed");
			cb_ctx[id].event = clCreateUserEvent(cl_ctx->context, &status);
			rt_assert(status == CL_SUCCESS, "clCreateUserEvent work_kernel failed");
			status = clSetEventCallback(read_event, CL_COMPLETE, work_callback, &cb_ctx[id]);
			rt_assert(status == CL_SUCCESS, "clSetEventCallback work_kernel failed");
			status = clReleaseEvent(read_event);
			rt_assert(status == CL_SUCCESS, "clReleaseEvent work_kernel failed");
		}
		status = clReleaseEvent(kernel_event[id]);
		rt_assert(status == CL_SUCCESS, "clReleaseEvent work_kernel failed");
		status = clWaitForEvents(1, &cb_ctx[id].event);
		rt_assert(status == CL_SUCCESS, "clWaitForEvents work_kernel failed");
		status = clReleaseEvent(cb_ctx[id].event);
		rt_assert(status == CL_SUCCESS, "clReleaseEvent work_kernel failed");
	}
	return (NULL);
}

static cl_mem
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

static cl_mem
	work_copy_ptr(struct s_opencl_ctx *cl_ctx, size_t size, void *ptr, int arg)
{
	cl_int	status;
	cl_mem	mem;

	mem = work_copy_array(cl_ctx, size, ptr);
	status = clSetKernelArg(cl_ctx->work_kernel, arg, sizeof(cl_mem), &mem);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg failed");
	return (mem);
}

static void
	work_destroy_array(cl_mem mem)
{
	cl_int	status;

	if (mem == NULL)
		return ;
	status = clReleaseMemObject(mem);
	rt_assert(status == CL_SUCCESS, "clReleaseMemObject failed");
}

static void
	work_create_buffers(t_work *work, struct s_opencl_ctx *cl_ctx)
{
	cl_int	status;

	cl_ctx->world_mem = work_copy_array(cl_ctx, offsetof(t_world, primitives), work->state->world);
	cl_ctx->ctx_mem = work_copy_array(cl_ctx, sizeof(*cl_ctx->ctx) * RT_WORK_OPENCL_GLOBAL_SIZE, cl_ctx->ctx);
	cl_ctx->result_mem[0] = clCreateBuffer(cl_ctx->context, CL_MEM_WRITE_ONLY, sizeof(*cl_ctx->result) * RT_WORK_OPENCL_CHUNK_SIZE, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer result[0] failed");
	cl_ctx->result_mem[1] = clCreateBuffer(cl_ctx->context, CL_MEM_WRITE_ONLY, sizeof(*cl_ctx->result) * RT_WORK_OPENCL_CHUNK_SIZE, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer result[1] failed");
	cl_ctx->indices_mem[0] = clCreateBuffer(cl_ctx->context, CL_MEM_READ_WRITE, sizeof(*cl_ctx->indices), NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer indices[0] failed");
	cl_ctx->indices_mem[1] = clCreateBuffer(cl_ctx->context, CL_MEM_READ_WRITE, sizeof(*cl_ctx->indices), NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer indices[1] failed");
	cl_ctx->primitives_mem = work_copy_ptr(cl_ctx, work->state->world->primitives_size, work->state->world->primitives, 6);
	cl_ctx->materials_mem = work_copy_ptr(cl_ctx, work->state->world->materials_size, work->state->world->materials, 7);
	cl_ctx->vertices_mem = work_copy_ptr(cl_ctx, work->state->world->vertices_size, work->state->world->vertices, 8);
	cl_ctx->accel_nodes_mem = work_copy_ptr(cl_ctx, work->state->world->accel_nodes_size, work->state->world->accel_nodes, 9);
	cl_ctx->accel_indices_mem = work_copy_ptr(cl_ctx, work->state->world->accel_indices_size, work->state->world->accel_indices, 10);
	cl_ctx->accel_degenerates_mem = work_copy_ptr(cl_ctx, work->state->world->accel_degenerates_size, work->state->world->accel_degenerates, 11);
	cl_ctx->texture_data_mem = work_copy_ptr(cl_ctx, work->state->world->texture_data_size, work->state->world->texture_data, 12);
	cl_ctx->textures_mem = work_copy_ptr(cl_ctx, work->state->world->textures_size, work->state->world->textures, 13);
	cl_ctx->bxdfs_mem = work_copy_ptr(cl_ctx, work->state->world->bxdfs_size, work->state->world->bxdfs, 14);
	cl_ctx->lights_mem = work_copy_ptr(cl_ctx, work->state->world->lights_size, work->state->world->lights, 15);
	status = clSetKernelArg(cl_ctx->work_kernel, 0, sizeof(cl_mem), &cl_ctx->world_mem);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 0 failed");
	status = clSetKernelArg(cl_ctx->work_kernel, 1, sizeof(cl_mem), &cl_ctx->ctx_mem);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg work_kernel 1 failed");
}

static void
	work_create_program(struct s_opencl_ctx *cl_ctx, cl_platform_id platform, cl_device_id device)
{
	char	*file;
	char	*string;
	size_t	length;
	cl_int	bin_status;
	cl_int	status;
	char	*error;
	char	*str;
	size_t	size;

	file = device_file(platform, device);
	string = rt_readfile(file, &error, &length);
	rt_free(file);
	rt_assert(string != NULL, error);
	cl_ctx->program = clCreateProgramWithBinary(cl_ctx->context, 1, &device, &length, (const unsigned char **) &string, &bin_status, &status);
	rt_assert(status == CL_SUCCESS && bin_status == CL_SUCCESS, "clCreateProgramWithBinary failed");
	status = clBuildProgram(cl_ctx->program, 1, &device, "", NULL, NULL);
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
	rt_free(string);
}

#ifdef RT_LINUX

static cl_command_queue
	work_create_queue(cl_context context, cl_device_id device, cl_command_queue_properties props)
{
	cl_int				status;
	cl_command_queue	result;
	cl_queue_properties	queue_props[3];

	queue_props[0] = CL_QUEUE_PROPERTIES;
	queue_props[1] = props;
	queue_props[2] = 0;
	if (props == 0)
		result = clCreateCommandQueueWithProperties(context, device, NULL, &status);
	else
		result = clCreateCommandQueueWithProperties(context, device, queue_props, &status);
	rt_assert(status == CL_SUCCESS, "clCreateCommandQueue failed");
	return (result);
}

#else

static cl_command_queue
	work_create_queue(cl_context context, cl_device_id device, cl_command_queue_properties props)
{
	cl_int				status;
	cl_command_queue	result;

	result = clCreateCommandQueue(context, device, props, &status);
	rt_assert(status == CL_SUCCESS, "clCreateCommandQueue failed");
	return (result);
}

#endif

static void
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
	cl_ctx->indices[0] = 0;
	cl_ctx->indices[1] = 0;
	context_props[0] = CL_CONTEXT_PLATFORM;
	context_props[1] = (cl_context_properties) platform;
	context_props[2] = 0;
	cl_ctx->context = clCreateContextFromType(context_props, CL_DEVICE_TYPE_ALL, NULL, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateContextFromType failed");
	cl_ctx->command_queue[0] = work_create_queue(cl_ctx->context, device, 0);
	cl_ctx->command_queue[1] = work_create_queue(cl_ctx->context, device, 0);
	work_create_program(cl_ctx, platform, device);
	cl_ctx->work_kernel = clCreateKernel(cl_ctx->program, "work_kernel", &status);
	rt_assert(status == CL_SUCCESS, "clCreateKernel work_kernel failed");
	cl_ctx->world_mem = NULL;
	cl_ctx->ctx_mem = NULL;
	cl_ctx->result_mem[0] = NULL;
	cl_ctx->result_mem[1] = NULL;
	cl_ctx->indices_mem[0] = NULL;
	cl_ctx->indices_mem[1] = NULL;
	cl_ctx->primitives_mem = NULL;
	cl_ctx->materials_mem = NULL;
	cl_ctx->vertices_mem = NULL;
	cl_ctx->accel_nodes_mem = NULL;
	cl_ctx->accel_indices_mem = NULL;
	cl_ctx->accel_degenerates_mem = NULL;
	cl_ctx->texture_data_mem = NULL;
	cl_ctx->textures_mem = NULL;
	cl_ctx->bxdfs_mem = NULL;
	cl_ctx->lights_mem = NULL;
	work_add(work, work_start, cl_ctx, RT_BACKEND_OPENCL);
}

static void
	work_int_create_opencl_default(t_work *work)
{
	cl_int			status;
	cl_platform_id	platform;
	cl_device_id	device;

	status = clGetPlatformIDs(1, &platform, NULL);
	rt_assert(status == CL_SUCCESS, "clGetPlatformIDs failed");
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device, NULL);
	rt_assert(status == CL_SUCCESS, "clGetDeviceIDs failed");
	work_setup(work, platform, device);
}

static void
	work_int_create_opencl_platform(t_work *work, cl_platform_id platform, int i)
{
	cl_int			status;
	cl_uint			num_devices;
	cl_device_id	*devices;

	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
	rt_assert(status == CL_SUCCESS, "clGetDeviceIDs failed");
	devices = rt_malloc(sizeof(*devices) * num_devices);
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);
	rt_assert(status == CL_SUCCESS, "clGetDeviceIDs failed");
	rt_assert(work->opts->cl_devices[i] < (int) num_devices, "opencl device index out of range");
	work_setup(work, platform, devices[work->opts->cl_devices[i]]);
	rt_free(devices);
}

void
	work_int_create_opencl(t_work *work)
{
	cl_int			status;
	cl_uint			num_platforms;
	cl_platform_id	*platforms;
	int				i;

	if (work->opts->cl_device_count == 0)
	{
		work_int_create_opencl_default(work);
		return ;
	}
	status = clGetPlatformIDs(0, NULL, &num_platforms);
	rt_assert(status == CL_SUCCESS, "clGetPlatformIDs failed");
	platforms = rt_malloc(sizeof(*platforms) * num_platforms);
	status = clGetPlatformIDs(num_platforms, platforms, NULL);
	rt_assert(status == CL_SUCCESS, "clGetPlatformIDs failed");
	i = 0;
	while (i < work->opts->cl_device_count)
	{
		rt_assert(work->opts->cl_platforms[i] < (int) num_platforms, "opencl platform index out of range");
		work_int_create_opencl_platform(work, platforms[work->opts->cl_platforms[i]], i);
		i += 1;
	}
	rt_free(platforms);
}

static void
    work_release_buffers(struct s_opencl_ctx *cl_ctx)
{
	work_destroy_array(cl_ctx->world_mem);
	work_destroy_array(cl_ctx->ctx_mem);
	work_destroy_array(cl_ctx->result_mem[0]);
	work_destroy_array(cl_ctx->result_mem[1]);
	work_destroy_array(cl_ctx->indices_mem[0]);
	work_destroy_array(cl_ctx->indices_mem[1]);
	work_destroy_array(cl_ctx->primitives_mem);
	work_destroy_array(cl_ctx->materials_mem);
	work_destroy_array(cl_ctx->vertices_mem);
	work_destroy_array(cl_ctx->accel_nodes_mem);
	work_destroy_array(cl_ctx->accel_indices_mem);
	work_destroy_array(cl_ctx->accel_degenerates_mem);
	work_destroy_array(cl_ctx->texture_data_mem);
	work_destroy_array(cl_ctx->textures_mem);
	work_destroy_array(cl_ctx->bxdfs_mem);
	work_destroy_array(cl_ctx->lights_mem);
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

#else

void
	work_int_create_opencl(t_work *work)
{
	(void) work;
	rt_assert(0, "not compiled with opencl support");
}

void
	work_int_destroy_opencl(t_work *work)
{
	(void) work;
	rt_assert(0, "not compiled with opencl support");
}

void
	work_int_resume_opencl(t_work *work)
{
	(void) work;
	rt_assert(0, "not compiled with opencl support");
}

#endif

