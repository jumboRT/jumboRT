#include "net.h"

#include "ser.h"
#include "z.h"
#include "util.h"
#include <string.h>
#include <math.h>

void
	*rt_upackhvec(void *src, t_vec *dst)
{
	float	x;
	float	y;
	float	z;

	src = rt_upackhfl(src, &x);
	src = rt_upackhfl(src, &y);
	src = rt_upackhfl(src, &z);
	*dst = vec(x, y, z, 0.0);
	return (src);
}

void
	*rt_upackvec(void *src, t_vec *dst)
{
	float	x;
	float	y;
	float	z;

	src = rt_upackfl(src, &x);
	src = rt_upackfl(src, &y);
	src = rt_upackfl(src, &z);
	*dst = vec(x, y, z, 0.0);
	return (src);
}
