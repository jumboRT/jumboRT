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
		rt_parse_error(ctx,
			"unexpected character %c expected %c", (int) *ctx->data, ch);
	rt_advance(ctx);
}

size_t
	rt_idlen(t_parse_ctx *ctx)
{
	return (rt_wordlen(ctx->data));
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
		rt_parse_error(ctx, "unexpected character %c expected int", *ctx->data);
	}
	result = ft_atol(ctx->data);
	if (result < 0 || (unsigned long) result > UINT_MAX)
	{
		rt_parse_error(ctx, "%.*s would not fit in unsigned int",
			rt_idlen(ctx), ctx->data);
	}
	rt_skip(ctx, ft_isdigit);
	return ((unsigned int) result);
}
