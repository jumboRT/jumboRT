#include "cl_impl.h"
#include "util.h"

#include <ft_printf.h>

#if RT_USE_OPENCL

static void
	opencl_check_program(t_opencl_program_ctx *ctx)
{
	cl_int	status;
	size_t	size;
	char	*string;

	status = clGetProgramBuildInfo(ctx->program, ctx->device,
			CL_PROGRAM_BUILD_LOG, 0, NULL, &size);
	rt_assert(status == CL_SUCCESS, "clGetProgramBuildInfo failed");
	string = rt_malloc(size);
	status = clGetProgramBuildInfo(ctx->program, ctx->device,
			CL_PROGRAM_BUILD_LOG, size, string, NULL);
	rt_assert(status == CL_SUCCESS, "clGetProgramBuildInfo failed");
	ft_printf("%s\n", string);
	rt_free(string);
}

void
	opencl_comp_program(t_opencl_program_ctx *ctx,
			const char *string, size_t length)
{
	cl_int		status;

	ctx->program = clCreateProgramWithSource(ctx->context,
			1, &string, &length, &status);
	rt_assert(status == CL_SUCCESS, "clCreateProgramWithSource failed");
	status = clCompileProgram(ctx->program, 1, &ctx->device,
			RT_WORK_OPENCL_BUILD_FLAGS, 0, NULL, NULL, NULL, NULL);
	opencl_check_program(ctx);
	rt_assert(status == CL_SUCCESS, "clBuildProgram failed");
}

void
	opencl_load_program(t_opencl_program_ctx *ctx,
			const unsigned char *string, size_t length, int build)
{
	cl_int		bin_status;
	cl_int		status;

	ctx->program = clCreateProgramWithBinary(ctx->context,
			1, &ctx->device, &length, &string, &bin_status, &status);
	rt_assert(status == CL_SUCCESS && bin_status == CL_SUCCESS,
			"clCreateProgramWithBinary failed");
	if (build)
	{
		status = clBuildProgram(ctx->program, 1, &ctx->device,
				RT_WORK_OPENCL_BUILD_FLAGS, NULL, NULL);
		opencl_check_program(ctx);
	}
	rt_assert(status == CL_SUCCESS, "clBuildProgram failed");
}

void
	opencl_link_program(t_opencl_program_ctx *ctx,
			cl_program *programs, size_t count)
{
	cl_int	status;

	ctx->program = clLinkProgram(ctx->context, 1, &ctx->device,
			RT_WORK_OPENCL_BUILD_FLAGS, count, programs, NULL, NULL, &status);
	opencl_check_program(ctx);
	rt_assert(status == CL_SUCCESS, "clLinkProgram failed");
}

void
	opencl_release_program(cl_program program)
{
	cl_int	status;

	status = clReleaseProgram(program);
	rt_assert(status == CL_SUCCESS, "clReleaseProgram failed");
}

#endif
