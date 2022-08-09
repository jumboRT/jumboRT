#include "world.h"

t_vec
	filter_sample_offset(const GLOBAL t_world *world, t_filter filter, t_vec2 uv, t_vec2 poffset)
{
	t_vec	tex1;
	t_vec	tex2;

	tex1 = sample_color_offset(world, filter.tex[0], uv, poffset);
	tex2 = sample_color_offset(world, filter.tex[1], uv, poffset);
	return (vec_mul(tex1, tex2));
}

t_vec
	filter_sample(const GLOBAL t_world *world, t_filter filter, t_vec2 uv)
{
	return (filter_sample_offset(world, filter, uv, vec2(0.0, 0.0)));
}
