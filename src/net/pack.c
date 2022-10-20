#include "ser.h"

void
	*rt_packvec(void *dst, t_vec vec)
{
	dst = rt_packfl(dst, x(vec));
	dst = rt_packfl(dst, y(vec));
	dst = rt_packfl(dst, z(vec));
	return (dst);
}

void
	*rt_packhvec(void *dst, t_vec vec)
{
	dst = rt_packhfl(dst, x(vec));
	dst = rt_packhfl(dst, y(vec));
	dst = rt_packhfl(dst, z(vec));
	return (dst);
}
