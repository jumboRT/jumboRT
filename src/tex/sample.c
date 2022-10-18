#include "tex.h"
#include "world.h"

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
