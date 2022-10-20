#include "cl_impl.h"
#include "util.h"

#if RT_USE_OPENCL

void
	opencl_comp_program_path(t_opencl_program_ctx *ctx, const char *path)
{
	char	*data;
	size_t	size;
	char	*error;

	error = NULL;
	data = rt_readfile(path, &error, &size);
	rt_assert(error == NULL, error);
	opencl_comp_program(ctx, data, size);
	rt_free(data);
	rt_free(error);
}

void
	opencl_load_program_path(t_opencl_program_ctx *ctx,
			const char *path, int build)
{
	char	*data;
	size_t	size;
	char	*error;

	error = NULL;
	path = device_file(path, ctx->platform, ctx->device);
	data = rt_readfile(path, &error, &size);
	rt_assert(error == NULL, error);
	opencl_load_program(ctx, (unsigned char *) data, size, build);
	rt_free(data);
	rt_free(error);
	rt_free((void *) path);
}

void
	opencl_link_program_path(t_opencl_program_ctx *ctx,
			const char *const *paths, size_t count)
{
	size_t		index;
	cl_program	*programs;

	programs = rt_malloc(count * sizeof(*programs));
	index = 0;
	while (index < count)
	{
		opencl_load_program_path(ctx, paths[index], 0);
		programs[index] = ctx->program;
		index += 1;
	}
	opencl_link_program(ctx, programs, index);
	index = 0;
	while (index < count)
	{
		opencl_release_program(programs[index]);
		index += 1;
	}
	rt_free(programs);
}

void
	opencl_save_program_path(t_opencl_program_ctx *ctx, const char *path)
{
	unsigned char	*data;
	size_t			size;
	char			*error;

	error = NULL;
	path = device_file(path, ctx->platform, ctx->device);
	data = opencl_save_program(ctx, &size);
	rt_writefile(path, &error, data, size);
	rt_assert(error == NULL, error);
	rt_free(data);
	rt_free(error);
	rt_free((void *) path);
}

#endif
