/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   world.c                                        #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:05 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:05 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#include <libft.h>

#if RT_BONUS

static const t_directive	g_directives[] = {
{"C", rt_exec_camera},
{"A", rt_exec_ambient},
{"sp", rt_exec_sphere},
{"pl", rt_exec_plane},
{"cy", rt_exec_cylinder},
{"L", rt_exec_light},
{"l", rt_exec_light},
{"co", rt_exec_cone},
{"pa", rt_exec_paraboloid},
{"hy", rt_exec_hyperboloid},
{"v", rt_exec_vertex},
{"w", rt_exec_vertex_texture},
{"y", rt_exec_vertex_normal},
{"x", rt_exec_vertex_texture_normal},
{"f", rt_exec_triangle},
{"tex_def", rt_exec_tex_def},
{"tex_def_filtered", rt_exec_tex_def_filtered},
{"checker_def", rt_exec_checker_def},
{"mat_use", rt_exec_mat_use},
{"mat_beg", rt_exec_mat_beg},
{"diffuse", rt_exec_diffuse},
{"reflective", rt_exec_reflective},
{"phong", rt_exec_phong},
{"oren_nayar", rt_exec_oren_nayar},
{"specular", rt_exec_specular},
{"transmissive", rt_exec_transmissive},
{"emission", rt_exec_emission},
{"emission_exp", rt_exec_emission_exp},
{"smooth", rt_exec_smooth},
{"alpha", rt_exec_alpha},
{"normal", rt_exec_normal},
{"bump", rt_exec_bump},
{"volume", rt_exec_volume},
{"mat_end", rt_exec_mat_end},
{"cam_focus", rt_exec_cam_focus},
{"cam_blur", rt_exec_cam_blur},
{"ambient_dist", rt_exec_ambient_dist},
{"refractive_index", rt_exec_refractive_index},
{"#", rt_exec_comment},
{"$", rt_exec_conditional},
{"!", rt_exec_include},
};

#else

static const t_directive	g_directives[] = {
{"C", rt_exec_camera},
{"A", rt_exec_ambient},
{"sp", rt_exec_sphere},
{"pl", rt_exec_plane},
{"cy", rt_exec_cylinder},
{"L", rt_exec_light},
};
#endif

static void
	rt_check_and_update(t_parse_ctx *ctx, char used[256], size_t i)
{
	if (ft_strlen(g_directives[i].name) == 1
		&& used[(unsigned char) *g_directives[i].name]
		&& ft_isupper(*g_directives[i].name))
		rt_parse_error(ctx, "multiple uses of directive %c",
			*g_directives[i].name);
	used[(unsigned char) *g_directives[i].name] = 1;
}

void
	rt_world(t_world *world, t_parse_ctx *ctx)
{
	size_t		len;
	size_t		i;
	char		used[256];

	ft_memset(used, 0, sizeof(used));
	rt_skip(ctx, ft_isspace);
	while (*ctx->data != '\0')
	{
		len = rt_idlen(ctx);
		i = 0;
		while (i < sizeof(g_directives) / sizeof(*g_directives))
		{
			if (ft_strlen(g_directives[i].name) == len
				&& ft_strncmp(g_directives[i].name, ctx->data, len) == 0)
				break ;
			i += 1;
		}
		rt_parse_assert(ctx, i != sizeof(g_directives) / sizeof(*g_directives),
			"unknown directive %.*s", len, ctx->data);
		rt_check_and_update(ctx, used, i);
		rt_idskip(ctx, len);
		g_directives[i].exec(world, ctx);
		rt_skip(ctx, ft_isspace);
	}
	rt_parse_assert(ctx, ctx->cam_set != 0, "scene has no camera");
}
