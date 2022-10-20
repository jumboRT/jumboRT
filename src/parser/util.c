#include "parser.h"

#include "util.h"

#include <stdarg.h>
#include <ft_printf.h>
#include <unistd.h>
#include <stdlib.h>
#include <libft.h>

void
	rt_parse_error(t_parse_ctx *ctx, const char *fmt, ...)
{
	va_list	args;
	char	*buffer;

	va_start(args, fmt);
	ft_vasprintf(&buffer, fmt, &args);
	va_end(args);
	ft_fprintf(STDERR_FILENO, "Error\n%s:%d:%d - %s\n",
		ctx->filename, ctx->line, ctx->column, buffer);
	rt_free(buffer);
	exit(EXIT_FAILURE);
}

void
	rt_parse_assert(t_parse_ctx *ctx, int cond, const char *fmt, ...)
{
	va_list	args;
	char	*buffer;

	if (cond)
		return ;
	va_start(args, fmt);
	ft_vasprintf(&buffer, fmt, &args);
	va_end(args);
	ft_fprintf(STDERR_FILENO, "Error\n%s:%d:%d - %s\n",
		ctx->filename, ctx->line, ctx->column, buffer);
	rt_free(buffer);
	exit(EXIT_FAILURE);
}

void
	rt_advance(t_parse_ctx *ctx)
{
	if (*ctx->data == '\0')
	{
		rt_parse_error(ctx, "cannot advance past end of file");
	}
	else if (*ctx->data == '\n')
	{
		ctx->line += 1;
		ctx->column = 1;
	}
	else
	{
		ctx->column += 1;
	}
	ctx->data += 1;
}

int
	has_prefix(const t_parse_ctx *ctx, const char *prefix)
{
	const char	*word;

	word = ctx->data;
	while (ft_isspace(*word))
		++word;
	return (ft_strncmp(word, prefix, ft_strlen(prefix)) == 0);
}

void
	rt_check_in_mat(t_parse_ctx *ctx)
{
	if (ctx->mat == NULL)
		rt_parse_error(ctx, "unexpected directive, did not start a material");
}
