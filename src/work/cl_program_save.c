#include "cl_impl.h"
#include "util.h"

#if RT_USE_OPENCL

static unsigned char
	**opencl_get_binaries(t_opencl_program_ctx *ctx, cl_uint num_devices,
			cl_device_id *devices, size_t **binary_sizes)
{
	unsigned char	**binaries;
	cl_int			status;
	cl_uint			index;

	(void) devices;
	*binary_sizes = rt_malloc(num_devices * sizeof(**binary_sizes));
	binaries = rt_malloc(num_devices * sizeof(*binaries));
	status = clGetProgramInfo(ctx->program, CL_PROGRAM_BINARY_SIZES,
			num_devices * sizeof(**binary_sizes), *binary_sizes, NULL);
	rt_assert(status == CL_SUCCESS, "clGetProgramInfo failed");
	index = 0;
	while (index < num_devices)
	{
		binaries[index] = rt_malloc((*binary_sizes)[index]);
		index += 1;
	}
	status = clGetProgramInfo(ctx->program, CL_PROGRAM_BINARIES,
			num_devices * sizeof(*binaries), binaries, NULL);
	rt_assert(status == CL_SUCCESS, "clGetProgramInfo failed");
	return (binaries);
}

static unsigned char
	*opencl_get_binary(t_opencl_program_ctx *ctx, size_t *length,
			cl_uint num_devices, cl_device_id *devices)
{
	unsigned char	**binaries;
	size_t			*binary_sizes;
	unsigned char	*result;
	cl_uint			index;

	binaries = opencl_get_binaries(ctx, num_devices, devices, &binary_sizes);
	index = 0;
	while (index < num_devices)
	{
		if (devices[index] == ctx->device)
		{
			result = rt_memdup(binaries[index], binary_sizes[index]);
			*length = binary_sizes[index];
		}
		rt_free(binaries[index]);
		index += 1;
	}
	rt_free(binaries);
	rt_free(binary_sizes);
	return (result);
}

unsigned char
	*opencl_save_program(t_opencl_program_ctx *ctx, size_t *length)
{
	cl_int			status;
	cl_uint			num_devices;
	cl_device_id	*devices;
	unsigned char	*result;

	status = clGetProgramInfo(ctx->program, CL_PROGRAM_NUM_DEVICES,
			sizeof(num_devices), &num_devices, NULL);
	rt_assert(status == CL_SUCCESS, "clGetProgramInfo failed");
	devices = rt_malloc(num_devices * sizeof(*devices));
	status = clGetProgramInfo(ctx->program, CL_PROGRAM_DEVICES,
			num_devices * sizeof(*devices), devices, NULL);
	rt_assert(status == CL_SUCCESS, "clGetProgramInfo failed");
	result = opencl_get_binary(ctx, length, num_devices, devices);
	rt_free(devices);
	return (result);
}

#endif
