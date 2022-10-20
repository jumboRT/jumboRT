#include <libft.h>

#include "rtmath.h"
#include <stdlib.h>
#include <math.h>

static float
	rt_magnitude(const char **str)
{
	float	magnitude;

	magnitude = 0.0;
	while (ft_isdigit(*(*str)))
	{
		magnitude = magnitude * 10 + ((float)(int)*(*str) - '0');
		*str += 1;
	}
	return (magnitude);
}

static float
	rt_mantissa(const char **str)
{
	float	mantissa;
	float	exp;

	mantissa = 0.0;
	exp = 10.0;
	while (ft_isdigit(*(*str)))
	{
		mantissa = mantissa + (((float)(int)*(*str) - '0') / exp);
		*str += 1;
		exp *= 10.0;
	}
	return (mantissa);
}

int
	rt_atof(const char *str, float *dst)
{
	float	magnitude;
	float	mantissa;
	float	sign;

	sign = 1.0;
	magnitude = 0.0;
	mantissa = 0.0;
	if (*str == '-')
	{
		sign *= -1.0;
		str++;
	}
	magnitude = rt_magnitude(&str);
	if (*str == '.')
	{
		str += 1;
		mantissa = rt_mantissa(&str);
	}
	*dst = (magnitude + mantissa) * sign;
	return (!isinf(*dst));
}
