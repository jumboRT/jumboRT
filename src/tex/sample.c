#include "tex.h"
#include "world.h"

static t_vec
	sample_texture(const GLOBAL t_world *world, t_tex tex, t_vec2 uv,
					t_vec2 offset)
{
	uint64_t					xv;
	uint64_t					yv;
	const GLOBAL unsigned char	*pixels;
	unsigned char				colors[4];

	uv = vec2(rt_mod(rt_mod(u(uv), 1.0) + 1.0, 1.0),
			rt_mod(rt_mod(v(uv), 1.0) + 1.0, 1.0));
	xv = ((uint64_t)(u(uv) * tex.a.tex.width)
			+ (uint64_t) u(offset)) % tex.a.tex.width;
	yv = ((uint64_t)(v(uv) * tex.a.tex.height)
			+ (uint64_t) v(offset)) % tex.a.tex.height;
	pixels = get_tex_data_const(world, tex.a.tex.offset);
	colors[0] = pixels[(yv * tex.a.tex.width + xv) * 4 + 0];
	colors[1] = pixels[(yv * tex.a.tex.width + xv) * 4 + 1];
	colors[2] = pixels[(yv * tex.a.tex.width + xv) * 4 + 2];
	colors[3] = pixels[(yv * tex.a.tex.width + xv) * 4 + 3];
	return (vec_scale(
				vec(colors[0], colors[1], colors[2], colors[3]), 1.0 / 255));
}

// static t_vec sample_checker(const GLOBAL t_world *world, t_tex tex,
// t_vec2 uv, t_vec2 offset)
static t_vec
	sample_checker(const GLOBAL t_world *world, t_tex tex, t_vec2 uv, t_vec2 offset)
{
	float sines;

	(void) world;
	sines = rt_sin(w(tex.a.checker.odd_color) * (u(uv) + u(offset)))
			* rt_sin(w(tex.a.checker.even_color) * (v(uv) + v(offset)));
	if (sines < 0.0)
		return (tex.a.checker.odd_color);
	return (tex.a.checker.even_color);
}

t_vec
	sample_vector_offset(const GLOBAL t_world *world, uint32_t id, t_vec2 uv,
			t_vec2 offset)
{
	const GLOBAL t_tex	*tex;

	tex = get_tex_const(world, id);
	if (tex->type == RT_TEX_COLOR)
		return (tex->a.color);
	if (tex->type == RT_TEX_CHECKER)
		return (sample_checker(world, *tex, uv, offset));
	return (sample_texture(world, *tex, uv, offset));
}

t_vec
	sample_color_offset(const GLOBAL t_world *world, uint32_t id, t_vec2 uv,
			t_vec2 offset)
{
	t_vec	v;

	v = sample_vector_offset(world, id, uv, offset);
	return (vec_igc(v));
}

float
	sample_float_offset(const GLOBAL t_world *world, uint32_t id, t_vec2 uv,
			t_vec2 offset)
{
	t_vec	v;

	v = sample_vector_offset(world, id, uv, offset);
	return (x(v));
}

t_vec
	sample_vector(const GLOBAL t_world *world, uint32_t id, t_vec2 uv)
{
	return (sample_vector_offset(world, id, uv, vec2(0, 0)));
}

t_vec
	sample_color(const GLOBAL t_world *world, uint32_t id, t_vec2 uv)
{
	return (sample_color_offset(world, id, uv, vec2(0, 0)));
}

float
	sample_float(const GLOBAL t_world *world, uint32_t id, t_vec2 uv)
{
	return (sample_float_offset(world, id, uv, vec2(0, 0)));
}

