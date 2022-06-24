#include "parser.h"

#include "util.h"
#include <libft.h>
#include <limits.h>

void
	rt_skip(t_parse_ctx *ctx, int (*pred)(int))
{
	while (pred(*ctx->data) && *ctx->data != '\0')
		rt_advance(ctx);
}

void
	rt_expect(t_parse_ctx *ctx, int ch)
{
	if (*ctx->data != ch)
		rt_parse_error(ctx, "unexpected character %c, expected %c", (int) *ctx->data, ch);
	rt_advance(ctx);
}

size_t
	rt_idlen(t_parse_ctx *ctx)
{
	size_t	i;

	i = 0;
	while (!ft_isspace(ctx->data[i]) && ctx->data[i] != '\0')
		i += 1;
	return (i);
}

void
	rt_idskip(t_parse_ctx *ctx, size_t len)
{
	while (len > 0)
	{
		rt_advance(ctx);
		len -= 1;
	}
}

unsigned int
	rt_uint(t_parse_ctx *ctx)
{
	long	result;

	rt_skip(ctx, ft_isspace);
	if (!ft_isdigit(*ctx->data))
	{
		rt_parse_error(ctx, "unexpected character %c, expected int", *ctx->data);
	}
	result = ft_atol(ctx->data);
	if (result < 0 || result > UINT_MAX)
	{
		rt_parse_error(ctx, "%.*s would not fit in unsigned int", rt_idlen(ctx), ctx->data);
	}
	rt_skip(ctx, ft_isdigit);
	return ((unsigned int) result);
}

FLOAT
	rt_float(t_parse_ctx *ctx)
{
	FLOAT	result;

	rt_skip(ctx, ft_isspace);
	if (!ft_isdigit(*ctx->data) && *ctx->data != '-')
	{
		rt_parse_error(ctx, "unexpected character %c, expected: float", *ctx->data);
	}
	if (!rt_atof(ctx->data, &result))
	{
		rt_parse_error(ctx, "%.*s would be infinite", rt_idlen(ctx), ctx->data);
	}
	if (*ctx->data == '-')
		rt_advance(ctx);
	rt_skip(ctx, ft_isdigit);
	if (*ctx->data == '.')
		rt_advance(ctx);
	rt_skip(ctx, ft_isdigit);
	return (result);
}

static FLOAT
	rt_color_part(t_parse_ctx *ctx)
{
	unsigned int	ival;

	ival = rt_uint(ctx);
	if (ival > 255)
	{
		rt_parse_error(ctx, "%d exceeds max value for color byte: 255", ival);
	}
	if (ival > 0)
		return ((FLOAT) ival / 255.0);
	return (0.0);
}

t_vec
	rt_color(t_parse_ctx *ctx)
{
	FLOAT	red;
	FLOAT	green;
	FLOAT	blue;

	rt_skip(ctx, ft_isspace);
	red = rt_color_part(ctx);
	rt_expect(ctx, ',');
	green = rt_color_part(ctx);
	rt_expect(ctx, ',');
	blue = rt_color_part(ctx);
	return (vec(red, green, blue));
}

t_vec
	rt_vec(t_parse_ctx *ctx)
{
	FLOAT	x;
	FLOAT	y;
	FLOAT	z;

	rt_skip(ctx, ft_isspace);
	x = rt_float(ctx);
	rt_expect(ctx, ',');
	y = rt_float(ctx);
	rt_expect(ctx, ',');
	z = rt_float(ctx);
	return (vec(x, y, z));
}

t_vec
	rt_vec_norm(t_parse_ctx *ctx)
{
	FLOAT	x;
	FLOAT	y;
	FLOAT	z;

	rt_skip(ctx, ft_isspace);
	x = rt_float(ctx);
	rt_expect(ctx, ',');
	y = rt_float(ctx);
	rt_expect(ctx, ',');
	z = rt_float(ctx);
	return (vec(x, y, z));
}
