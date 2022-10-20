#include "parser.h"

#include <libft.h>

void
	rt_exec_conditional(t_world *world, t_parse_ctx *ctx)
{
	size_t	len;

	(void) world;
	rt_skip(ctx, ft_isspace);
	len = rt_idlen(ctx);
	if (ft_strlen(ctx->key) != len || ft_strncmp(ctx->key, ctx->data, len) != 0)
		while (*ctx->data != '\n' && *ctx->data != '\0')
			rt_advance(ctx);
	else
		rt_idskip(ctx, len);
}
