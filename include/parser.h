#ifndef PARSER_H
# define PARSER_H

# include "world.h"

# include <stddef.h>

typedef struct s_directive	t_directive;
typedef struct s_parse_ctx	t_parse_ctx;

struct s_directive {
	const char	*name;
	void		(*exec)(t_world *world, t_parse_ctx *ctx);
};

struct s_parse_ctx {
	const char	*data;
	const char	*filename;
	int			line;
	int			column;
};

void			rt_parse_error(t_parse_ctx *ctx, const char *fmt, ...);
void			rt_advance(t_parse_ctx *ctx);

void			rt_skip(t_parse_ctx *ctx, int (*pred)(int));
void			rt_expect(t_parse_ctx *ctx, int ch);
size_t			rt_idlen(t_parse_ctx *ctx);
void			rt_idskip(t_parse_ctx *ctx, size_t len);

FLOAT			rt_float(t_parse_ctx *ctx);
int				rt_bool(t_parse_ctx *ctx);
unsigned int	rt_uint(t_parse_ctx *ctx);
t_vec			rt_color(t_parse_ctx *ctx);
t_vec			rt_vec(t_parse_ctx *ctx);
t_vec			rt_vec_norm(t_parse_ctx *ctx);
char			*rt_keyword(t_parse_ctx *ctx, const char *prefix);
FLOAT			rt_float_range(t_parse_ctx *ctx, FLOAT min, FLOAT max);
void			rt_material(t_parse_ctx *ctx, t_world *world, t_primitive *shape);

void			init_camera(t_world *world);

void			rt_exec_camera(t_world *world, t_parse_ctx *ctx);
void			rt_exec_sphere(t_world *world, t_parse_ctx *ctx);
void			rt_exec_plane(t_world *world, t_parse_ctx *ctx);
void			rt_exec_vertex(t_world *world, t_parse_ctx *ctx);
void			rt_exec_triangle(t_world *world, t_parse_ctx *ctx);
void			rt_exec_cylinder(t_world *world, t_parse_ctx *ctx);
void			rt_exec_cone(t_world *world, t_parse_ctx *ctx);
void			rt_exec_light(t_world *world, t_parse_ctx *ctx);
void			rt_exec_material(t_world *world, t_parse_ctx *ctx);
void			rt_exec_comment(t_world *world, t_parse_ctx *ctx);

void			rt_world(t_world *world, t_parse_ctx *ctx);

#endif
