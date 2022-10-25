/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vec.c                                          #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:06 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:06 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include <libft.h>

t_vec
	rt_vec(t_parse_ctx *ctx)
{
	float	x;
	float	y;
	float	z;

	x = rt_float(ctx);
	rt_expect(ctx, ',');
	y = rt_float(ctx);
	rt_expect(ctx, ',');
	z = rt_float(ctx);
	return (vec(x, y, z, 0.0));
}

t_vec2
	rt_vec2(t_parse_ctx *ctx)
{
	float	x;
	float	y;

	rt_skip(ctx, ft_isspace);
	x = rt_float(ctx);
	rt_expect(ctx, ',');
	rt_skip(ctx, ft_isspace);
	y = rt_float(ctx);
	return (vec2(x, y));
}

t_vec
	rt_vec_norm(t_parse_ctx *ctx)
{
	t_vec	v;

	v = rt_vec(ctx);
	if (vec_eq(v, vec_0()))
		rt_parse_error(ctx, "vector may not be the zero vector");
	return (vec_norm(v));
}
