/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   word.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:06 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:06 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <libft.h>

size_t
	rt_wordlen(const char *str)
{
	size_t	i;

	i = 0;
	while (!ft_isspace(str[i]) && str[i] != '\0')
		i += 1;
	return (i);
}

size_t
	rt_wordnlen(const char *str, size_t n)
{
	size_t	i;

	i = rt_wordlen(str);
	if (i > n)
		i = n;
	return (i);
}

char
	*rt_word(t_parse_ctx *ctx)
{
	char	*result;
	size_t	id_len;

	rt_skip(ctx, ft_isspace);
	id_len = rt_idlen(ctx);
	if (id_len == 0)
	{
		rt_parse_error(ctx, "expected word, got nothing");
	}
	result = ft_strndup(ctx->data, id_len);
	rt_idskip(ctx, id_len);
	return (result);
}

char
	*rt_keyword(t_parse_ctx *ctx, const char *prefix)
{
	size_t	prefix_len;
	size_t	id_len;
	char	*result;

	rt_skip(ctx, ft_isspace);
	prefix_len = ft_strlen(prefix);
	id_len = rt_idlen(ctx);
	if (ft_strncmp(ctx->data, prefix, prefix_len))
	{
		rt_parse_error(ctx, "unexpected keyword %.*s, expected keyword"
			" starting with: '%s'", id_len, ctx->data, prefix);
	}
	result = ft_strndup(ctx->data, id_len);
	rt_idskip(ctx, id_len);
	return (result);
}
