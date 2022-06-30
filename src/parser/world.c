#include "parser.h"

#include <libft.h>

const static t_directive	directives[] = {
	{ "C", rt_exec_camera },
	{ "sp", rt_exec_sphere },
	{ "v", rt_exec_vertex },
	{ "tr", rt_exec_triangle },
	{ "#", rt_exec_comment },
};

void
	rt_world(t_world *world, t_parse_ctx *ctx)
{
	size_t		len;
	size_t		i;

	rt_skip(ctx, ft_isspace);
	while (*ctx->data != '\0')
	{
		len = rt_idlen(ctx);
		i = 0;
		while (i < sizeof(directives) / sizeof(*directives))
		{
			if (ft_strlen(directives[i].name) == len
				&& ft_strncmp(directives[i].name, ctx->data, len) == 0)
				break ;
			i += 1;
		}
		if (i == sizeof(directives) / sizeof(*directives))
			rt_parse_error(ctx, "unknown directive %.*s", len, ctx->data);
		rt_idskip(ctx, len);
		directives[i].exec(world, ctx);
		rt_skip(ctx, ft_isspace);
	}
}

