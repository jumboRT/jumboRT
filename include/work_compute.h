/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   work_compute.h                                 #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:53 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:53 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORK_COMPUTE_H
# define WORK_COMPUTE_H

# include "world.h"
# include "types.h"

t_result	work_compute(GLOBAL t_world *world, GLOBAL t_context *ctx,
				t_uint64 index);

#endif
