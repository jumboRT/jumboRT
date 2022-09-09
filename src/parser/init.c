#include "parser.h"


#include "util.h"
#include <ft_printf.h>
#include <stdlib.h>

void
	parser_init(t_parse_ctx *ctx, const char *filename, const char *key)
{
	char	*error;

	ctx->data = rt_readfile(filename, &error, &ctx->size);
	if (ctx->data == NULL)
	{
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
	ctx->cam_set = 0;
	ctx->max_index = 0;
	if (ctx->key == NULL)
		ctx->key = "default";
	vector_create(&ctx->materials, sizeof(t_entry), 0);
	vector_create(&ctx->textures, sizeof(t_entry), 0);
}

void
	parser_push(t_parse_ctx *ctx, const char *filename)
{
	char	*error;
	char	*include_data;
	size_t	include_size;
	char	*merged_data;

	include_data = rt_readfile(filename, &error, &include_size);
	if (include_data == NULL)
	{
		ft_fprintf(STDERR_FILENO, "Error\n%s\n", error);
		rt_free(error);
		exit(EXIT_FAILURE);
	}
	merged_data = rt_malloc(ctx->size + include_size + 3);
	rt_memcpy(merged_data, ctx->begin, ctx->data - ctx->begin);
	merged_data[ctx->data - ctx->begin] = '\n';
	rt_memcpy(merged_data + (ctx->data - ctx->begin) + 1, include_data, include_size);
	merged_data[ctx->data - ctx->begin + include_size + 1] = '\n';
	rt_memcpy(merged_data + (ctx->data - ctx->begin) + include_size + 2, ctx->data, ctx->begin + ctx->size - ctx->data);
	merged_data[ctx->size + include_size + 2] = '\0';
	rt_free(ctx->begin);
	rt_free(include_data);
	ctx->data = merged_data + (ctx->data - ctx->begin);
	ctx->begin = merged_data;
	ctx->size = ctx->size + include_size + 2;
}

static void
	destroy_entry(void *ctx)
{
	t_entry	*entry;

	entry = ctx;
	rt_free(entry->name);
}

void
	parser_destroy(t_parse_ctx *ctx)
{
	vector_destroy(&ctx->materials, destroy_entry);
	vector_destroy(&ctx->textures, destroy_entry);
	rt_free(ctx->begin);
}
