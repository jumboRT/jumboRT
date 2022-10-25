/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   comment.c                                      #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:07 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:07 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void
	rt_exec_comment(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	while (*ctx->data != '\n' && *ctx->data != '\0')
		rt_advance(ctx);
}
