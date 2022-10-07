#include "cl_impl.h"
#include "util.h"

#if RT_USE_OPENCL

# ifdef RT_LINUX

cl_command_queue
	work_create_queue(cl_context context, cl_device_id device,
			cl_command_queue_properties props)
{
	cl_int				status;
	cl_command_queue	result;
	cl_queue_properties	queue_props[3];

	queue_props[0] = CL_QUEUE_PROPERTIES;
	queue_props[1] = props;
	queue_props[2] = 0;
	if (props == 0)
		result = clCreateCommandQueueWithProperties(
				context, device, NULL, &status);
	else
		result = clCreateCommandQueueWithProperties(
				context, device, queue_props, &status);
	rt_assert(status == CL_SUCCESS,
			"clCreateCommandQueueWithProperties failed");
	return (result);
}

# else

cl_command_queue
	work_create_queue(cl_context context, cl_device_id device,
			cl_command_queue_properties props)
{
	cl_int				status;
	cl_command_queue	result;

	result = clCreateCommandQueue(context, device, props, &status);
	rt_assert(status == CL_SUCCESS, "clCreateCommandQueue failed");
	return (result);
}

# endif

cl_context
	work_create_context(cl_platform_id platform)
{
	cl_context_properties	context_props[3];
	cl_int					status;
	cl_context				result;

	context_props[0] = CL_CONTEXT_PLATFORM;
	context_props[1] = (cl_context_properties) platform;
	context_props[2] = 0;
	result = clCreateContextFromType(context_props, CL_DEVICE_TYPE_ALL,
			NULL, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateContextFromType failed");
	return (result);
}

void
	work_destroy_queue(cl_command_queue queue)
{
	cl_int	status;

	status = clReleaseCommandQueue(queue);
	rt_assert(status == CL_SUCCESS, "clReleaseQueue failed");
}

void
	work_destroy_context(cl_context context)
{
	cl_int	status;

	status = clReleaseContext(context);
	rt_assert(status == CL_SUCCESS, "clReleaseContext failed");
}

#endif
