/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   random1.c                                      #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:16 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:16 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rand.h"

t_seed
	rt_random(GLOBAL t_seed *seed)
{
	*seed ^= *seed << 12;
	*seed ^= *seed >> 25;
	*seed ^= *seed << 27;
	return (*seed * (((t_seed) 0x2545F491 << 32) | 0x4F6CDD1D));
}

float
	rt_random_float(GLOBAL t_seed *seed)
{
	return ((rt_random(seed) & 0xFFFFFF) / 16777216.0);
}

float
	rt_random_gauss(GLOBAL t_seed *seed)
{
	float	sum;
	int		idx;

	sum = 0.0f;
	idx = 0;
	while (idx < 12)
	{
		sum += rt_random_float(seed);
		idx++;
	}
	return (6.0f - sum);
}

float
	rt_random_float_range(GLOBAL t_seed *seed, float min, float max)
{
	return (min + (max - min) * (rt_random_float(seed)));
}

t_vec
	rt_random_in_sphere(GLOBAL t_seed *seed)
{
	t_vec	result;
	float	len;

	while (1)
	{
		result = vec(
				rt_random_float_range(seed, -1, 1),
				rt_random_float_range(seed, -1, 1),
				rt_random_float_range(seed, -1, 1), 0.0);
		len = vec_mag(result);
		if (len <= 1)
			return (result);
	}
}
