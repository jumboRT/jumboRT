#ifndef PARSER_H
# define PARSER_H

# include "world.h"
# include "vector.h"

# include <stddef.h>

typedef struct s_directive	t_directive;
typedef struct s_entry		t_entry;
typedef struct s_parse_ctx	t_parse_ctx;

struct s_directive {
	const char	*name;
	void		(*exec)(t_world *world, t_parse_ctx *ctx);
};

struct s_entry {
	uint32_t	index;
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
	uint32_t	mat_use;
	const char	*key;
};

int				has_prefix(const t_parse_ctx *ctx, const char *prefix);

void			parser_init(t_parse_ctx *ctx, const char *filename, const char *key);
void			parser_push(t_parse_ctx *ctx, const char *filename);
void			parser_destroy(t_parse_ctx *ctx);
void			mat_add(t_parse_ctx *ctx, const char *name, uint32_t index);
void			tex_add(t_parse_ctx *ctx, const char *name, uint32_t index);
uint32_t		mat_by_name(t_world *world, t_parse_ctx *ctx, const char *name);
uint32_t		mat_by_color(t_world *world, t_parse_ctx *ctx, t_vec color);
uint32_t		tex_by_name(t_world *world, t_parse_ctx *ctx, const char *name);
uint32_t		tex_by_color(t_world *world, t_parse_ctx *ctx, t_vec color);

void			rt_parse_error(t_parse_ctx *ctx, const char *fmt, ...);
void			rt_advance(t_parse_ctx *ctx);

void			rt_skip(t_parse_ctx *ctx, int (*pred)(int));
void			rt_expect(t_parse_ctx *ctx, int ch);
size_t			rt_idlen(t_parse_ctx *ctx);
void			rt_idskip(t_parse_ctx *ctx, size_t len);

float			rt_float(t_parse_ctx *ctx);
int				rt_bool(t_parse_ctx *ctx);
unsigned int	rt_uint(t_parse_ctx *ctx);
t_vec			rt_color(t_parse_ctx *ctx);
t_vec			rt_vec(t_parse_ctx *ctx);
t_vec2			rt_vec2(t_parse_ctx *ctx);
t_vec			rt_vec_norm(t_parse_ctx *ctx);
char			*rt_word(t_parse_ctx *ctx);
char			*rt_keyword(t_parse_ctx *ctx, const char *prefix);
float			rt_float_range(t_parse_ctx *ctx, float min, float max);
uint32_t		rt_texture(t_world *world, t_parse_ctx *ctx);
t_filter		rt_filter(t_world *world, t_parse_ctx *ctx);
void			rt_material(t_parse_ctx *ctx, t_world *world, t_primitive *shape);
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
void			rt_exec_light(t_world *world, t_parse_ctx *ctx);
void			rt_exec_mat_use(t_world *world, t_parse_ctx *ctx);
void			rt_exec_mat_beg(t_world *world, t_parse_ctx *ctx);
void			rt_exec_mat_end(t_world *world, t_parse_ctx *ctx);
void			rt_exec_tex_def(t_world *world, t_parse_ctx *ctx);
void			rt_exec_checker_def(t_world *world, t_parse_ctx *ctx);
void			rt_exec_diffuse(t_world *world, t_parse_ctx *ctx);
void			rt_exec_reflective(t_world *world, t_parse_ctx *ctx);
void			rt_exec_cook_torrance(t_world *world, t_parse_ctx *ctx);
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
void			rt_exec_comment(t_world *world, t_parse_ctx *ctx);
void			rt_exec_conditional(t_world *world, t_parse_ctx *ctx);
void			rt_exec_include(t_world *world, t_parse_ctx *ctx);

void			rt_world(t_world *world, t_parse_ctx *ctx);

void			world_load(t_world *world, const char *filename, const char *key);

#endif
