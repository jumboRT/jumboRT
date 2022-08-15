#include "parser.h"


#include "util.h"

#include <stdarg.h>
#include <ft_printf.h>
#include <unistd.h>
#include <stdlib.h>

/* TODO: don't use varargs in mandatory */
void
	rt_parse_error(t_parse_ctx *ctx, const char *fmt, ...)
{
	va_list	args;
	char	*buffer;

	va_start(args, fmt);
	ft_vasprintf(&buffer, fmt, &args);
	va_end(args);
	ft_fprintf(STDERR_FILENO, "Error\n%s:%d:%d - %s\n", ctx->filename, ctx->line, ctx->column, buffer);
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

