#include "parser.h"
#include <libft.h>

void
	rt_strtof_init(const char *str, float ifes[4], char **end,
			int *seen_digit)
{
	*seen_digit = 0;
	ifes[3] = 1.0f;
	ifes[0] = 0.0f;
	ifes[1] = 0.0f;
	ifes[2] = 1.0f;
	*end = (char *) str;
}

float
	rt_strtof(const char *str, char **end)
{
	float	ifes[4];
	int	seen_digit;

	rt_strtof_init(str, ifes, end, &seen_digit);
	if (*str == '-')
	{
		ifes[3] = -1.0f;
		str++;
	}
	while (ft_isdigit(*str))
	{
		ifes[0] = (ifes[0] * 10.0f) + (*str++ - '0');
		seen_digit = 1;
	}
	str += *str == '.';
	while (ft_isdigit(*str))
	{
		ifes[2] /= 10.0f;
		ifes[1] += (*str++ - '0') * ifes[2];
		seen_digit = 1;
	}
	if (seen_digit)
		*end = (char *) str;
	return ((ifes[0] + ifes[1]) * ifes[3]);
}

float
	rt_float(t_parse_ctx *ctx)
{
	float	val;
	char	*end;

	rt_skip(ctx, ft_isspace);
	val = rt_strtof(ctx->data, &end);
	if (end - ctx->begin == 0)
		rt_parse_error(ctx, "bad floating point value");
	if (!isfinite(val))
		rt_parse_error(ctx,
			"could not represent value as finite number");
	ctx->data = end;
	return (val);
}

float
	rt_float_range(t_parse_ctx *ctx, float min, float max)
{
	float		result;
	const char	*word;

	word = ctx->data;
	result = rt_float(ctx);
	if (result < min || result > max)
	{
		rt_parse_error(ctx, "float out of range '%.*s'",
				(int) rt_wordnlen(word, 64), word);
	}
	return (result);
}
