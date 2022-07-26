
#include <libft.h>

#include "rtmath.h"
#include <stdlib.h>
#include <math.h>

static FLOAT
	rt_magnitude(const char **str)
{
	FLOAT	magnitude;

	magnitude = 0.0;
	while (ft_isdigit(*(*str)))
	{
		magnitude = magnitude * 10 + ((FLOAT) (int) *(*str) - '0');
		*str += 1;
	}
	return (magnitude);
}

static FLOAT
	rt_mantissa(const char **str)
{
	FLOAT	mantissa;
	FLOAT	exp;

	mantissa = 0.0;
	exp = 10.0;
	while (ft_isdigit(*(*str)))
	{
		mantissa = mantissa + (((FLOAT) (int) *(*str) - '0') / exp);
		*str += 1;
		exp *= 10.0;
	}
	return (mantissa);
}

int
	rt_atof(const char *str, FLOAT *dst)
{
	FLOAT	magnitude;
	FLOAT	mantissa;
	FLOAT	sign;

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

