/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   hit.h                                          #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:49 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:49 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef HIT_H
# define HIT_H

# include "typedefs.h"
# include "rtmath.h"
# include "types.h"

struct s_world_hit {
	t_hit						hit;
	const GLOBAL t_primitive	*prim;
	const GLOBAL t_material		*mat;
	int							is_volume;
	int							is_ambient;
};

#endif
