/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   context.c                                      #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:08 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:08 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "work.h"

void
	ctx_init(t_context *ctx)
{
	static t_seed	seed = 7549087012;

	while (1)
	{
		ctx->seed = rt_random(&seed);
		if (ctx->seed != 0)
			break ;
	}
}

void
	ctx_destroy(t_context *ctx)
{
	(void) ctx;
}
