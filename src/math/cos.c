/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cos.c                                          #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:24 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:24 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#ifndef RT_OPENCL

float
	rt_cos(float a)
{
	return (cosf(a));
}

float
	rt_acos(float a)
{
	return (acosf(a));
}

#else

float
	rt_cos(float a)
{
	return (cos(a));
}

float
	rt_acos(float a)
{
	return (acos(a));
}

#endif
