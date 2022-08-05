#include "parser.h"

#include <libft.h>

const static t_directive	directives[] = {
	{ "C", rt_exec_camera },
	{ "sp", rt_exec_sphere },
	{ "pl", rt_exec_plane },
	{ "cy", rt_exec_cylinder },
	{ "co", rt_exec_cone },
	{ "v", rt_exec_vertex },
	{ "w", rt_exec_vertex_texture },
	{ "V", rt_exec_vertex_normal },
	{ "W", rt_exec_vertex_texture_normal },
	{ "f", rt_exec_triangle },
	{ "L", rt_exec_light },
	{ "tex_def", rt_exec_tex_def },
	{ "mat_use", rt_exec_mat_use },
	{ "mat_beg", rt_exec_mat_beg },
	{ "diffuse", rt_exec_diffuse },
	{ "reflective", rt_exec_reflective },
	{ "mf_reflective", rt_exec_mf_reflective },
	{ "refractive", rt_exec_refractive },
	{ "emission", rt_exec_emission },
	{ "smooth", rt_exec_smooth },
	{ "alpha", rt_exec_alpha },
	{ "normal", rt_exec_normal },
	{ "bump", rt_exec_bump },
	{ "volume", rt_exec_volume },
	{ "mat_end", rt_exec_mat_end },
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

