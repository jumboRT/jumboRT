#include "rtmath.h"

#if defined RT_VECTORIZE

t_vec
	vec_min(t_vec a, t_vec b)
{
	t_vec_conv	conv_a;
	t_vec_conv	conv_b;

	conv_a.vec = a;
	conv_b.vec = b;
	conv_a.elem[0] = rt_min(conv_a.elem[0], conv_b.elem[0]);
	conv_a.elem[1] = rt_min(conv_a.elem[1], conv_b.elem[1]);
	conv_a.elem[2] = rt_min(conv_a.elem[2], conv_b.elem[2]);
	conv_a.elem[3] = rt_min(conv_a.elem[3], conv_b.elem[3]);
	return (conv_a.vec);
}

t_vec
	vec_max(t_vec a, t_vec b)
{
	t_vec_conv	conv_a;
	t_vec_conv	conv_b;

	conv_a.vec = a;
	conv_b.vec = b;
	conv_a.elem[0] = rt_max(conv_a.elem[0], conv_b.elem[0]);
	conv_a.elem[1] = rt_max(conv_a.elem[1], conv_b.elem[1]);
	conv_a.elem[2] = rt_max(conv_a.elem[2], conv_b.elem[2]);
	conv_a.elem[3] = rt_max(conv_a.elem[3], conv_b.elem[3]);
	return (conv_a.vec);
}

t_vec
	vec_clamp(t_vec v, FLOAT min, FLOAT max)
{
	t_vec_conv	conv;

	conv.vec = v;
	conv.elem[0] = rt_max(rt_min(conv.elem[0], max), min);
	conv.elem[1] = rt_max(rt_min(conv.elem[1], max), min);
	conv.elem[2] = rt_max(rt_min(conv.elem[2], max), min);
	conv.elem[3] = rt_max(rt_min(conv.elem[3], max), min);
	return (conv.vec);
}

#endif
