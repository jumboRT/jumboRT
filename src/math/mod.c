/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   mod.c                                          #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:22 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:22 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#ifndef RT_OPENCL

float
	rt_mod(float a, float b)
{
	return (fmod(a, b));
}

#else

float
	rt_mod(float a, float b)
{
	return (fmod(a, b));
}

#endif
