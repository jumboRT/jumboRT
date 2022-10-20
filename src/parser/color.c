#include "parser.h"
#include <libft.h>

static float
	rt_color_part(t_parse_ctx *ctx)
{
	unsigned int	ival;

	rt_skip(ctx, ft_isspace);
	ival = rt_uint(ctx);
	if (ival > 255)
	{
		rt_parse_error(ctx, "%d exceeds max value for color byte: 255", ival);
	}
	if (ival > 0)
		return ((float) ival / 255.0);
	return (0.0);
}

t_vec
	rt_fcolor(t_parse_ctx *ctx)
{
	t_vec	result;

	rt_skip(ctx, ft_isspace);
	rt_expect(ctx, '(');
	result = rt_vec(ctx);
	result = vec_set(result, 3, 1.0);
	if (*ctx->data == ',')
	{
		rt_advance(ctx);
		result = vec_set(result, 3, rt_float(ctx));
	}
	rt_skip(ctx, ft_isspace);
	rt_expect(ctx, ')');
	return (result);
}

t_vec
	rt_color(t_parse_ctx *ctx)
{
	float	red;
	float	green;
	float	blue;

	rt_skip(ctx, ft_isspace);
	if (RT_BONUS && *ctx->data == '(')
		return rt_fcolor(ctx);
	red = rt_color_part(ctx);
	rt_expect(ctx, ',');
	green = rt_color_part(ctx);
	rt_expect(ctx, ',');
	blue = rt_color_part(ctx);
	return (vec(red, green, blue, 1.0));
}
