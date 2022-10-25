/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   rand.h                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:51 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:51 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef RAND_H
# define RAND_H
# include "types.h"

# include "rtmath.h"

typedef t_uint64	t_seed;

t_seed	rt_random(GLOBAL t_seed *seed);
float	rt_random_float(GLOBAL t_seed *seed);
float	rt_random_float_range(GLOBAL t_seed *seed, float min, float max);
t_vec	rt_random_in_sphere(GLOBAL t_seed *seed);
t_vec	rt_random_on_hemi(GLOBAL t_seed *seed, t_vec normal);
t_vec	rt_random_in_disk(GLOBAL t_seed *seed, t_vec right, t_vec up,
			float radius);
float	rt_random_gauss(GLOBAL t_seed *seed);
t_vec	rt_random_unit_sphere(GLOBAL t_seed *seed);

t_vec2	rt_random_concentric_disk(GLOBAL t_seed *seed);
t_vec	rt_random_cosine_hemi(GLOBAL t_seed *seed);

#endif
