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

# include "rtmath.h"
# include "types.h"

typedef struct s_material	t_material;
typedef struct s_primitive	t_primitive;

typedef struct s_world_hit	t_world_hit;
typedef struct s_light_hit	t_light_hit;

struct s_world_hit {
	t_hit						hit;
	const GLOBAL t_primitive	*prim;
	const GLOBAL t_material		*mat;
	int							is_volume;
	int							is_ambient;
};

#endif
