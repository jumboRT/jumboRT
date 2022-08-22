#include "parser.h"


#include "util.h"
#include <ft_printf.h>
#include <stdlib.h>

void
	parser_init(t_parse_ctx *ctx, const char *filename, const char *key)
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
	ctx->mat_use_set = 0;
	ctx->key = key;
	if (ctx->key == NULL)
		ctx->key = "default";
	vector_create(&ctx->materials, sizeof(t_mat_entry), 0);
	vector_create(&ctx->textures, sizeof(t_tex_entry), 0);
}

/* TODO: free individual materials */
void
	parser_destroy(t_parse_ctx *ctx)
{
	vector_destroy(&ctx->materials, NULL);
	vector_destroy(&ctx->textures, NULL);
	rt_free(ctx->begin);
}
