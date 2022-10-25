/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   parser.h                                       #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:50 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:50 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "typedefs.h"
# include "world.h"
# include "vector.h"
# include "tex.h"

# include <stddef.h>

struct s_directive {
	const char	*name;
	void		(*exec)(t_world *world, t_parse_ctx *ctx);
};

struct s_entry {
	t_uint32	index;
	char		*name;
	t_vec		color;
};

struct s_parse_ctx {
	char		*begin;
	char		*data;
	size_t		size;
	const char	*filename;
	int			line;
	int			column;
	int			cam_set;
	t_material	*mat;
	t_vector	materials;
	t_vector	textures;
	int			mat_use_set;
	t_uint32	mat_use;
	size_t		max_index;
	const char	*key;
};

int				has_prefix(const t_parse_ctx *ctx, const char *prefix);

void			parser_init(t_parse_ctx *ctx, const char *filename,
					const char *key);
void			parser_push(t_parse_ctx *ctx, const char *filename);
void			parser_destroy(t_parse_ctx *ctx);
void			mat_add(t_parse_ctx *ctx, const char *name, t_uint32 index);
void			tex_add(t_parse_ctx *ctx, const char *name, t_uint32 index);
t_uint32		mat_by_name(t_world *world, t_parse_ctx *ctx, const char *name);
t_uint32		mat_by_color(t_world *world, t_parse_ctx *ctx, t_vec color);
t_uint32		tex_by_name(t_world *world, t_parse_ctx *ctx, const char *name);
t_uint32		tex_by_color(t_world *world, t_parse_ctx *ctx, t_vec color);
void			rt_check_in_mat(t_parse_ctx *ctx);

void			rt_parse_assert(t_parse_ctx *ctx, int cond, const char *fmt,
					...);
void			rt_parse_error(t_parse_ctx *ctx, const char *fmt, ...);
void			rt_advance(t_parse_ctx *ctx);

void			rt_skip(t_parse_ctx *ctx, int (*pred)(int));
void			rt_expect(t_parse_ctx *ctx, int ch);
size_t			rt_wordlen(const char *str);
size_t			rt_wordnlen(const char *str, size_t n);
size_t			rt_idlen(t_parse_ctx *ctx);
void			rt_idskip(t_parse_ctx *ctx, size_t len);

float			rt_float(t_parse_ctx *ctx);
unsigned int	rt_uint(t_parse_ctx *ctx);
t_vec			rt_fcolor(t_parse_ctx *ctx);
t_vec			rt_color(t_parse_ctx *ctx);
t_vec			rt_vec(t_parse_ctx *ctx);
t_vec2			rt_vec2(t_parse_ctx *ctx);
t_vec			rt_vec_norm(t_parse_ctx *ctx);
char			*rt_word(t_parse_ctx *ctx);
char			*rt_keyword(t_parse_ctx *ctx, const char *prefix);
float			rt_float_range(t_parse_ctx *ctx, float min, float max);
t_uint32		rt_texture(t_world *world, t_parse_ctx *ctx);
t_filter		rt_filter(t_world *world, t_parse_ctx *ctx);
void			rt_material(t_parse_ctx *ctx, t_world *world,
					t_primitive *shape);
void			rt_tex(t_parse_ctx *ctx, t_world *world, t_material *material);

void			rt_exec_camera(t_world *world, t_parse_ctx *ctx);
void			rt_exec_ambient(t_world *world, t_parse_ctx *ctx);
void			rt_exec_sphere(t_world *world, t_parse_ctx *ctx);
void			rt_exec_plane(t_world *world, t_parse_ctx *ctx);
void			rt_exec_vertex(t_world *world, t_parse_ctx *ctx);
void			rt_exec_vertex_texture(t_world *world, t_parse_ctx *ctx);
void			rt_exec_vertex_normal(t_world *world, t_parse_ctx *ctx);
void			rt_exec_vertex_texture_normal(t_world *world, t_parse_ctx *ctx);
void			rt_exec_triangle(t_world *world, t_parse_ctx *ctx);
void			rt_exec_cylinder(t_world *world, t_parse_ctx *ctx);
void			rt_exec_cone(t_world *world, t_parse_ctx *ctx);
void			rt_exec_paraboloid(t_world *world, t_parse_ctx *ctx);
void			rt_exec_hyperboloid(t_world *world, t_parse_ctx *ctx);
void			rt_exec_light(t_world *world, t_parse_ctx *ctx);
void			rt_exec_mat_use(t_world *world, t_parse_ctx *ctx);
void			rt_exec_mat_beg(t_world *world, t_parse_ctx *ctx);
void			rt_exec_mat_end(t_world *world, t_parse_ctx *ctx);
void			rt_exec_tex_def(t_world *world, t_parse_ctx *ctx);
void			rt_exec_tex_def_filtered(t_world *world, t_parse_ctx *ctx);
void			rt_exec_checker_def(t_world *world, t_parse_ctx *ctx);
void			rt_exec_diffuse(t_world *world, t_parse_ctx *ctx);
void			rt_exec_reflective(t_world *world, t_parse_ctx *ctx);
void			rt_exec_phong(t_world *world, t_parse_ctx *ctx);
void			rt_exec_oren_nayar(t_world *world, t_parse_ctx *ctx);
void			rt_exec_specular(t_world *world, t_parse_ctx *ctx);
void			rt_exec_transmissive(t_world *world, t_parse_ctx *ctx);
void			rt_exec_emission(t_world *world, t_parse_ctx *ctx);
void			rt_exec_emission_exp(t_world *world, t_parse_ctx *ctx);
void			rt_exec_smooth(t_world *world, t_parse_ctx *ctx);
void			rt_exec_alpha(t_world *world, t_parse_ctx *ctx);
void			rt_exec_normal(t_world *world, t_parse_ctx *ctx);
void			rt_exec_bump(t_world *world, t_parse_ctx *ctx);
void			rt_exec_volume(t_world *world, t_parse_ctx *ctx);
void			rt_exec_mat_end(t_world *world, t_parse_ctx *ctx);
void			rt_exec_cam_focus(t_world *world, t_parse_ctx *ctx);
void			rt_exec_cam_blur(t_world *world, t_parse_ctx *ctx);
void			rt_exec_ambient_dist(t_world *world, t_parse_ctx *ctx);
void			rt_exec_refractive_index(t_world *world, t_parse_ctx *ctx);
void			rt_exec_comment(t_world *world, t_parse_ctx *ctx);
void			rt_exec_conditional(t_world *world, t_parse_ctx *ctx);
void			rt_exec_include(t_world *world, t_parse_ctx *ctx);

void			rt_world(t_world *world, t_parse_ctx *ctx);

void			world_load(t_world *world, const char *filename,
					const char *key);

#endif
