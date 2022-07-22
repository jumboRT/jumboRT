#include "world.h"

t_vec
	tex_sample(const GLOBAL t_world *world, const GLOBAL t_tex *tex, t_vec2 uv)
{
	uint64_t			x;
	uint64_t			y;
	const unsigned char	*pixels;
	unsigned char		colors[3];

	x = (uint64_t)(u(uv) * tex->width);
	y = (uint64_t)(v(uv) * tex->height);
	if (x >= tex->width)
		x = tex->width - 1;
	if (y >= tex->height)
		y = tex->height - 1;
	y = tex->height - 1 - y;
	pixels = get_tex_data_const(world, tex->offset);
	colors[0] = pixels[(y * tex->width + x) * 3 + 0];
	colors[1] = pixels[(y * tex->width + x) * 3 + 1];
	colors[2] = pixels[(y * tex->width + x) * 3 + 2];
	return (vec_scale(vec(colors[0], colors[1], colors[2]), 1.0 / 255));
}
