#include "world.h"

t_vec
	tex_sample_id(const GLOBAL t_world *world, uint32_t texidx, t_vec2 uv)
{
	const GLOBAL t_tex	*tex;

	tex = get_tex_const(world, texidx);
	if (tex->type == RT_TEX_COLOR) {
		return (tex->a.color);
	} else {
		return tex_sample(world, tex, uv);
	}
}

FLOAT
	tex_samplef_id_offset(const GLOBAL t_world *world, uint32_t texidx, t_vec2 uv, t_vec2 poffset)
{
	const GLOBAL t_tex	*tex;

	tex = get_tex_const(world, texidx);
	if (tex->type == RT_TEX_COLOR) {
		return (x(tex->a.color));
	} else {
		return x(tex_sample_offset(world, tex, uv, poffset));
	}
}

t_vec
	tex_sample_offset(const GLOBAL t_world *world, const GLOBAL t_tex *tex, t_vec2 uv, t_vec2 poffset)
{
	uint64_t					xv;
	uint64_t					yv;
	const GLOBAL unsigned char	*pixels;
	unsigned char				colors[4];
	t_vec						color;

	uv = vec2(rt_mod(rt_mod(u(uv), 1.0) + 1.0, 1.0), rt_mod(rt_mod(v(uv), 1.0) + 1.0, 1.0));
	xv = ((uint64_t)(u(uv) * tex->a.tex.width) + (uint64_t) u(poffset)) % tex->a.tex.width;
	yv = ((uint64_t)(v(uv) * tex->a.tex.height) + (uint64_t) v(poffset)) % tex->a.tex.height;
	pixels = get_tex_data_const(world, tex->a.tex.offset);
	colors[0] = pixels[(yv * tex->a.tex.width + xv) * 4 + 0];
	colors[1] = pixels[(yv * tex->a.tex.width + xv) * 4 + 1];
	colors[2] = pixels[(yv * tex->a.tex.width + xv) * 4 + 2];
	colors[3] = pixels[(yv * tex->a.tex.width + xv) * 4 + 3];
	color = vec_scale(vec(colors[0], colors[1], colors[2], colors[3]), 1.0 / 255);
	return (vec(x(color) * x(color), y(color) * y(color), z(color) * z(color), w(color)));
}

t_vec
	tex_sample(const GLOBAL t_world *world, const GLOBAL t_tex *tex, t_vec2 uv)
{
	return (tex_sample_offset(world, tex, uv, vec2(0, 0)));
}
