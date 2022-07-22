#include "parser.h"

#include "util.h"
#include <ft_printf.h>
#include <stdlib.h>

void
	parser_init(t_parse_ctx *ctx, const char *filename)
{
	char	*error;

	ctx->data = rt_readfile(filename, &error, NULL);
	if (ctx->data == NULL) {
		ft_fprintf(STDERR_FILENO, "Error\n%s\n", error);
		rt_free(error);
		exit(EXIT_FAILURE);
	}
	ctx->begin = ctx->data;
	ctx->filename = filename;
	ctx->line = 1;
	ctx->column = 1;
	ctx->mat = NULL;
	vector_create(&ctx->materials, sizeof(t_mat_entry), 0);
	vector_create(&ctx->textures, sizeof(t_tex_entry), 0);
}

void
	parser_destroy(t_parse_ctx *ctx)
{
	vector_destroy(&ctx->materials, NULL);
	vector_destroy(&ctx->textures, NULL);
	rt_free(ctx->begin);
}
