#include "rtmath.h"

FLOAT
	rt_clamp(FLOAT arg, FLOAT min, FLOAT max)
{
	if (arg <= min)
		return (min);
	else if (arg >= max)
		return (max);
	return (arg);
}
