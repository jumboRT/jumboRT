#include "world.h"

t_vec
	tex_sample(const GLOBAL t_world *world, const GLOBAL t_tex *tex, t_vec2 uv)
{
	uint64_t			x;
	uint64_t			y;
	const unsigned char	*pixels;
	unsigned char		colors[4];

	uv = vec2(rt_mod(rt_mod(u(uv), 1.0) + 1.0, 1.0), rt_mod(rt_mod(v(uv), 1.0) + 1.0, 1.0));
	x = (uint64_t)(u(uv) * tex->width);
	y = (uint64_t)(v(uv) * tex->height);
	pixels = get_tex_data_const(world, tex->offset);
	colors[0] = pixels[(y * tex->width + x) * 3 + 0];
	colors[1] = pixels[(y * tex->width + x) * 3 + 1];
	colors[2] = pixels[(y * tex->width + x) * 3 + 2];
	colors[3] = pixels[(y * tex->width + x) * 3 + 3];
	return (vec_scale(vec(colors[0], colors[1], colors[2], colors[3]), 1.0 / 255));
}
