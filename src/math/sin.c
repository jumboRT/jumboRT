/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   sin.c                                          #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:20 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:20 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#ifndef RT_OPENCL

float
	rt_sin(float a)
{
	return (sinf(a));
}

float
	rt_asin(float a)
{
	return (asinf(a));
}

#else

float
	rt_sin(float a)
{
	return (sin(a));
}

float
	rt_asin(float a)
{
	return (asin(a));
}

#endif
