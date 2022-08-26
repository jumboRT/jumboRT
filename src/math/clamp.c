#include "rtmath.h"

float
	rt_clamp(float arg, float min, float max)
{
	if (arg <= min)
		return (min);
	else if (arg >= max)
		return (max);
	return (arg);
}
