/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   aabb.h                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:37 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:37 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef AABB_H
# define AABB_H

# include "typedefs.h"
# include "rtmath.h"

struct s_bounds {
	t_vec	min;
	t_vec	max;
};

t_bounds	bounds(t_vec min, t_vec max);
t_bounds	bounds_0(void);
t_bounds	bounds_max(t_bounds a, t_bounds b);
float		bounds_surf(t_bounds a);

#endif
