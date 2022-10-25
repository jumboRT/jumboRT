/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   clamp.c                                        #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:21 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:21 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

float
	rt_clamp(float arg, float min, float max)
{
	if (arg <= min)
		return (min);
	else if (arg >= max)
		return (max);
	return (arg);
}
