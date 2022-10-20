#include "cl_impl.h"
#include "util.h"

#include <libft.h>

#if RT_USE_OPENCL

void
	opencl_build_program(t_opencl_program_ctx *ctx,
			const char **strings, const size_t *lengths, size_t count)
{
	cl_int	status;

	ctx->program = clCreateProgramWithSource(ctx->context,
			count, strings, lengths, &status);
	rt_assert(status == CL_SUCCESS, "clCreateProgramWithSource failed");
	status = clBuildProgram(ctx->program, 1, &ctx->device,
			RT_WORK_OPENCL_BUILD_FLAGS, NULL, NULL);
	opencl_check_program(ctx, ctx->program);
	rt_assert(status == CL_SUCCESS, "clBuildProgram failed");
}

void
	opencl_build_program_path(t_opencl_program_ctx *ctx,
			const char *const *paths, size_t count)
{
	size_t	index;
	char	**strings;
	size_t	*lengths;
	char	*error;

	strings = rt_malloc(count * sizeof(*strings));
	lengths = rt_malloc(count * sizeof(*lengths));
	index = 0;
	while (index < count)
	{
		strings[index] = rt_readfile(paths[index], &error, &lengths[index]);
		rt_assert(strings[index] != NULL, error);
		index += 1;
	}
	opencl_build_program(ctx, (const char **) strings, lengths, count);
	index = 0;
	while (index < count)
	{
		rt_free(strings[index]);
		index += 1;
	}
	rt_free(strings);
	rt_free(lengths);
}

static void
	opencl_build_int(void *data, cl_platform_id platform, cl_device_id device)
{
	t_opencl_program_ctx	pctx;
	t_opencl_compile_ctx	*cctx;
	size_t					index;
	const char *const		*input;

	cctx = data;
	if (!opencl_check_cache(cctx, platform, device))
		return ;
	pctx.platform = platform;
	pctx.device = device;
	pctx.context = work_create_context(platform);
	input = cctx->input;
	index = 0;
	while (input[index] != NULL)
		index += 1;
	opencl_build_program_path(&pctx, input, index);
	opencl_save_program_path(&pctx, cctx->output);
	work_destroy_context(pctx.context);
}

void
	opencl_build(const char **i, const char *o)
{
	t_opencl_compile_ctx	ctx;
	size_t					index;

	ctx.cache = rt_malloc(sizeof(*ctx.cache));
	ctx.cache[0] = NULL;
	ctx.input = i;
	ctx.output = o;
	opencl_device_all(opencl_build_int, &ctx);
	index = 0;
	while (ctx.cache[index] != NULL)
	{
		rt_free(ctx.cache[index]);
		index += 1;
	}
	rt_free(ctx.cache);
}

#endif
