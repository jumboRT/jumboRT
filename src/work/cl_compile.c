#include "cl_impl.h"
#include "util.h"

#include <libft.h>

#if RT_USE_OPENCL

static void
	opencl_compile_int(void *data, cl_platform_id platform, cl_device_id device)
{
	t_opencl_program_ctx	pctx;
	t_opencl_compile_ctx	*cctx;

	cctx = data;
	if (!opencl_check_cache(cctx, platform, device))
		return ;
	pctx.platform = platform;
	pctx.device = device;
	pctx.context = work_create_context(platform);
	opencl_comp_program_path(&pctx, cctx->input);
	opencl_save_program_path(&pctx, cctx->output);
	work_destroy_context(pctx.context);
}

static void
	opencl_link_int(void *data, cl_platform_id platform, cl_device_id device)
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
	opencl_link_program_path(&pctx, input, index);
	opencl_save_program_path(&pctx, cctx->output);
	work_destroy_context(pctx.context);
}

void
	opencl_compile(const char *i, const char *o)
{
	t_opencl_compile_ctx	ctx;
	size_t					index;

	ctx.cache = rt_malloc(sizeof(*ctx.cache));
	ctx.cache[0] = NULL;
	ctx.input = i;
	ctx.output = o;
	opencl_device_all(opencl_compile_int, &ctx);
	index = 0;
	while (ctx.cache[index] != NULL)
	{
		rt_free(ctx.cache[index]);
		index += 1;
	}
	rt_free(ctx.cache);
}

void
	opencl_link(const char **i, const char *o)
{
	t_opencl_compile_ctx	ctx;
	size_t					index;

	ctx.cache = rt_malloc(sizeof(*ctx.cache));
	ctx.cache[0] = NULL;
	ctx.input = i;
	ctx.output = o;
	opencl_device_all(opencl_link_int, &ctx);
	index = 0;
	while (ctx.cache[index] != NULL)
	{
		rt_free(ctx.cache[index]);
		index += 1;
	}
	rt_free(ctx.cache);
}

#endif
