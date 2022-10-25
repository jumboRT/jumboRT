/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cl_device.c                                    #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:10 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:10 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "cl_impl.h"
#include "util.h"

#if RT_USE_OPENCL

void
	opencl_device_default(t_opencl_func func, void *data)
{
	cl_int			status;
	cl_platform_id	platform;
	cl_device_id	device;

	status = clGetPlatformIDs(
			1, &platform, NULL);
	rt_assert(status == CL_SUCCESS, "clGetPlatformIDs failed");
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT,
			1, &device, NULL);
	rt_assert(status == CL_SUCCESS, "clGetDeviceIDs failed");
	func(data, platform, device);
}

static void
	opencl_device_by_index_int(cl_platform_id platform, int di,
			t_opencl_func func, void *data)
{
	cl_int			status;
	cl_uint			num_devices;
	cl_device_id	*devices;

	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,
			0, NULL, &num_devices);
	rt_assert(status == CL_SUCCESS, "clGetDeviceIDs failed");
	devices = rt_malloc(sizeof(*devices) * num_devices);
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,
			num_devices, devices, NULL);
	rt_assert(status == CL_SUCCESS, "clGetDeviceIDs failed");
	rt_assert(di < (int) num_devices, "opencl device index out of range");
	func(data, platform, devices[di]);
	rt_free(devices);
}

void
	opencl_device_by_index(int pi, int di,
			t_opencl_func func, void *data)
{
	cl_int			status;
	cl_uint			num_platforms;
	cl_platform_id	*platforms;

	status = clGetPlatformIDs(
			0, NULL, &num_platforms);
	rt_assert(status == CL_SUCCESS, "clGetPlatformIDs failed");
	platforms = rt_malloc(sizeof(*platforms) * num_platforms);
	status = clGetPlatformIDs(
			num_platforms, platforms, NULL);
	rt_assert(status == CL_SUCCESS, "clGetPlatformIDs failed");
	rt_assert(pi < (int) num_platforms, "opencl platform index out of range");
	opencl_device_by_index_int(platforms[pi], di, func, data);
	rt_free(platforms);
}

static void
	opencl_device_all_int(cl_platform_id platform,
			t_opencl_func func, void *data)
{
	cl_int			status;
	cl_uint			num_devices;
	cl_device_id	*devices;
	cl_uint			di;

	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,
			0, NULL, &num_devices);
	rt_assert(status == CL_SUCCESS, "clGetDeviceIDs failed");
	devices = rt_malloc(sizeof(*devices) * num_devices);
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,
			num_devices, devices, NULL);
	rt_assert(status == CL_SUCCESS, "clGetDeviceIDs failed");
	di = 0;
	while (di < num_devices)
	{
		func(data, platform, devices[di]);
		di += 1;
	}
	rt_free(devices);
}

void
	opencl_device_all(
			t_opencl_func func, void *data)
{
	cl_int			status;
	cl_uint			num_platforms;
	cl_platform_id	*platforms;
	cl_uint			pi;

	status = clGetPlatformIDs(
			0, NULL, &num_platforms);
	rt_assert(status == CL_SUCCESS, "clGetPlatformIDs failed");
	platforms = rt_malloc(sizeof(*platforms) * num_platforms);
	status = clGetPlatformIDs(
			num_platforms, platforms, NULL);
	rt_assert(status == CL_SUCCESS, "clGetPlatformIDs failed");
	pi = 0;
	while (pi < num_platforms)
	{
		opencl_device_all_int(platforms[pi], func, data);
		pi += 1;
	}
	rt_free(platforms);
}

#endif
