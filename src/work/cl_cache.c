/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cl_cache.c                                     #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:10 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:10 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "cl_impl.h"
#include "util.h"

#include <libft.h>

#if RT_USE_OPENCL

int
	opencl_check_cache(t_opencl_compile_ctx *ctx,
			cl_platform_id platform, cl_device_id device)
{
	char					*tmp;
	size_t					index;

	tmp = device_token(platform, device);
	index = 0;
	while (ctx->cache[index] != NULL)
	{
		if (ft_strcmp(ctx->cache[index], tmp) == 0)
		{
			rt_free(tmp);
			return (0);
		}
		index += 1;
	}
	ctx->cache = rt_realloc(ctx->cache,
			(index + 1) * sizeof(*ctx->cache),
			(index + 2) * sizeof(*ctx->cache));
	ctx->cache[index] = tmp;
	ctx->cache[index + 1] = NULL;
	return (1);
}

#endif
