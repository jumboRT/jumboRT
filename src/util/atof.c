#include <libft.h>

static double
	rt_magnitude(const char **str)
{
	double	magnitude;

	magnitude = 0.0;
	while (ft_isdigit(*(*str)))
	{
		magnitude = magnitude * 10 + ((double) (int) *(*str) - '0');
		*str += 1;
	}
	return (magnitude);
}

static double
	rt_mantissa(const char **str)
{
	double	mantissa;
	double	exp;

	mantissa = 0.0;
	exp = 10.0;
	while (ft_isdigit(*(*str)))
	{
		mantissa = mantissa + (((double) (int) *(*str) - '0') / exp);
		*str += 1;
		exp *= 10.0;
	}
	return (mantissa);
}

int
	rt_atof(const char *str, double *dst)
{
	double	magnitude;
	double	mantissa;
	double	sign;

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
	return (*str == '\0');
}

/*
int
	main(int argc, char **argv)
{
	double	result;
	int		valid;

	if (argc != 2)
		return 1;
	valid = rt_atof(argv[1], &result);
	fprintf(stdout, "'%s' valid?:%d double:'%.10f'\n", argv[1], valid, result);
	return 0;
}
*/
