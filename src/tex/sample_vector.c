#include "tex.h"
#include "world.h"

static float
	rt_fmod(float x, float y)
{
	return (rt_mod(rt_mod(x, y) + y, y));
}

static t_vec
	sample_texture(const GLOBAL t_world *world, t_tex tex, t_vec2 uv,
					t_vec2 offset)
{
	uint64_t					xv;
	uint64_t					yv;
	const GLOBAL unsigned char	*pixels;
	unsigned char				colors[4];

	uv = vec2(rt_fmod(u(uv), 1.0f), rt_fmod(v(uv), 1.0f));
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
	sample_checker(const GLOBAL t_world *world,
			t_tex tex, t_vec2 uv, t_vec2 offset)
{
	float	sines;

	(void) world;
	sines = rt_sin(w(tex.a.checker.odd_color) * (u(uv) + u(offset)))
		* rt_sin(w(tex.a.checker.even_color) * (v(uv) + v(offset)));
	if (sines < 0.0)
		return (tex.a.checker.odd_color);
	return (tex.a.checker.even_color);
}

static t_vec
	sample_texture_filtered(const GLOBAL t_world *world,
			t_tex tex, t_vec2 uv, t_vec2 offset)
{
	t_vec	corners[4];
	float	scale[2];
	t_vec	edges[2];

	scale[0] = 1.0f - rt_fmod(u(uv) * tex.a.tex.width, 1.0f);
	scale[1] = 1.0f - rt_fmod(v(uv) * tex.a.tex.height, 1.0f);
	corners[0] = sample_texture(world, tex, uv, vec2_add(offset, vec2(0, 0)));
	corners[1] = sample_texture(world, tex, uv, vec2_add(offset, vec2(1, 0)));
	corners[2] = sample_texture(world, tex, uv, vec2_add(offset, vec2(0, 1)));
	corners[3] = sample_texture(world, tex, uv, vec2_add(offset, vec2(1, 1)));
	corners[0] = vec_scale(corners[0], 0.0f + scale[0]);
	corners[1] = vec_scale(corners[1], 1.0f - scale[0]);
	corners[2] = vec_scale(corners[2], 0.0f + scale[0]);
	corners[3] = vec_scale(corners[3], 1.0f - scale[0]);
	edges[0] = vec_add(corners[0], corners[1]);
	edges[1] = vec_add(corners[2], corners[3]);
	edges[0] = vec_scale(edges[0], 0.0f + scale[1]);
	edges[1] = vec_scale(edges[1], 1.0f - scale[1]);
	return (vec_add(edges[0], edges[1]));
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
	if (tex->type == RT_TEX_FILTERED)
		return (sample_texture_filtered(world, *tex, uv, offset));
	return (sample_texture(world, *tex, uv, offset));
}
