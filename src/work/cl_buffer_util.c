#include "cl_impl.h"
#include "util.h"

#if RT_USE_OPENCL

cl_mem
	work_copy_ptr(t_opencl_ctx *cl_ctx, size_t size, void *ptr, int arg)
{
	cl_int	status;
	cl_mem	mem;

	if (size == 0)
	{
		status = clSetKernelArg(cl_ctx->work_kernel, arg, sizeof(cl_mem), NULL);
		rt_assert(status == CL_SUCCESS, "clSetKernelArg failed");
		return (NULL);
	}
	mem = clCreateBuffer(cl_ctx->context,
			CL_MEM_READ_ONLY, size, NULL, &status);
	rt_assert(status == CL_SUCCESS, "clCreateBuffer failed");
	/* ODOT: async copy may be significantly faster for multi gpu systems */
	status = clEnqueueWriteBuffer(cl_ctx->command_queue[0],
			mem, CL_TRUE, 0, size, ptr, 0, NULL, NULL);
	rt_assert(status == CL_SUCCESS, "clEnqueueWriteBuffer failed");
	status = clSetKernelArg(cl_ctx->work_kernel, arg, sizeof(cl_mem), &mem);
	rt_assert(status == CL_SUCCESS, "clSetKernelArg failed");
	return (mem);
}

void
	work_destroy_ptr(cl_mem mem)
{
	cl_int	status;

	if (mem == NULL)
		return ;
	status = clReleaseMemObject(mem);
	rt_assert(status == CL_SUCCESS, "clReleaseMemObject failed");
}

#endif

