#include "parser.h"
#include "util.h"
#include "memory.h"

#include <libft.h>

void
	rt_exec_include(t_world *world, t_parse_ctx *ctx)
{
	char	*file;
	size_t	len;

	(void) world;
	rt_skip(ctx, ft_isspace);
	len = rt_idlen(ctx);
	file = ft_strndup(ctx->data, len);
	rt_idskip(ctx, len);
	parser_push(ctx, file);
	rt_free(file);
}
