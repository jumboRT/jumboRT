#ifndef TEX_H
# define TEX_H

# include "types.h"
# include "rtmath.h"

# define RT_TEX_COLOR		0
# define RT_TEX_TEXTURE		1
# define RT_TEX_CHECKER		2
# define RT_TEX_FILTERED	3

typedef struct s_world	t_world;

typedef struct s_tex	t_tex;
typedef struct s_filter	t_filter;

struct s_filter {
	uint32_t	tex[2];
};

struct s_hwo {
	uint64_t	width;
	uint64_t	height;
	uint32_t	offset;
};

struct s_checker {
	t_vec		odd_color;
	t_vec		even_color;
};

union u_tex_extra {
	t_vec				color;
	struct s_hwo		tex;
	struct s_checker	checker;
};

struct s_tex {
	uint32_t			type;
	union u_tex_extra	a;
};

t_vec	filter_sample_offset(const GLOBAL t_world *world, t_filter filter,
			t_vec2 uv, t_vec2 poffset);
t_vec	filter_sample(const GLOBAL t_world *world, t_filter filter,
			t_vec2 uv);
t_vec	sample_vector_offset(const GLOBAL t_world *world, uint32_t id,
			t_vec2 uv, t_vec2 offset);
t_vec	sample_color_offset(const GLOBAL t_world *world, uint32_t id,
			t_vec2 uv, t_vec2 offset);
float	sample_float_offset(const GLOBAL t_world *world, uint32_t id,
			t_vec2 uv, t_vec2 offset);
t_vec	sample_vector(const GLOBAL t_world *world, uint32_t id, t_vec2 uv);
t_vec	sample_color(const GLOBAL t_world *world, uint32_t id, t_vec2 uv);
float	sample_float(const GLOBAL t_world *world, uint32_t id, t_vec2 uv);

void	texture_init(t_tex *tex);

#endif
