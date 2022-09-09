#include "parser.h"

#include <libft.h>

static const t_directive	directives[] = {
	{ "C", rt_exec_camera },
	{ "A", rt_exec_ambient },
	{ "sp", rt_exec_sphere },
	{ "pl", rt_exec_plane },
	{ "cy", rt_exec_cylinder },
	{ "L", rt_exec_light },
#ifdef RT_BONUS
	{ "l", rt_exec_light },
	{ "co", rt_exec_cone },
	{ "v", rt_exec_vertex },
	{ "w", rt_exec_vertex_texture },
	{ "y", rt_exec_vertex_normal },
	{ "x", rt_exec_vertex_texture_normal },
	{ "f", rt_exec_triangle },
	{ "tex_def", rt_exec_tex_def },
	{ "checker_def", rt_exec_checker_def },
	{ "mat_use", rt_exec_mat_use },
	{ "mat_beg", rt_exec_mat_beg },
	{ "diffuse", rt_exec_diffuse },
	{ "reflective", rt_exec_reflective },
	{ "cook_torrance", rt_exec_cook_torrance },
	{ "transmissive", rt_exec_transmissive },
	{ "emission", rt_exec_emission },
	{ "emission_exp", rt_exec_emission_exp },
	{ "smooth", rt_exec_smooth },
	{ "alpha", rt_exec_alpha },
	{ "normal", rt_exec_normal },
	{ "bump", rt_exec_bump },
	{ "volume", rt_exec_volume },
	{ "mat_end", rt_exec_mat_end },
	{ "cam_focus", rt_exec_cam_focus },
	{ "cam_blur", rt_exec_cam_blur },
	{ "#", rt_exec_comment },
	{ "$", rt_exec_conditional },
	{ "!", rt_exec_include },
#endif
};

void
	rt_world(t_world *world, t_parse_ctx *ctx)
{
	size_t		len;
	size_t		i;
	char used[256];

	ft_memset(used, 0, sizeof(used));
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
		if (ft_strlen(directives[i].name) == 1 && used[(unsigned char) *directives[i].name]
		    && ft_isupper(*directives[i].name))
			rt_parse_error(ctx, "multiple uses of directive %c", *directives[i].name);
		used[(unsigned char) *directives[i].name] = 1;
		rt_idskip(ctx, len);
		directives[i].exec(world, ctx);
		rt_skip(ctx, ft_isspace);
	}
}

