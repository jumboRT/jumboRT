#include "cl.h"

t_seed
	rt_random(GLOBAL t_seed *seed)
{
	*seed ^= *seed << 12;
	*seed ^= *seed >> 25;
	*seed ^= *seed << 27;
	return (*seed * (((t_seed) 0x2545F491 << 32) | 0x4F6CDD1D));
}

FLOAT
	rt_random_float(GLOBAL t_seed *seed)
{
	return (rt_random(seed) / 18446744073709551616.0);
}

FLOAT
	rt_random_float_range(GLOBAL t_seed *seed, FLOAT min, FLOAT max)
{
	return (min + (max - min) * (rt_random_float(seed)));
}

