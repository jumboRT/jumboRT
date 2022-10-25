/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   mat.h                                          #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:50 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:50 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAT_H
# define MAT_H

# include "types.h"
# include "bsdf.h"
# include "tex.h"

# define RT_MAT_SMOOTH				1
# define RT_MAT_EMITTER				2
# define RT_MAT_HAS_ALPHA			4
# define RT_MAT_HAS_VOLUME			8
# define RT_MAT_HAS_NORMAL			16
# define RT_MAT_HAS_BUMP			32

typedef struct s_material			t_material;

struct s_material {
	t_uint32	flags;
	t_bsdf		surface;
	t_bsdf		volume;
	t_filter	alpha;
	t_filter	emission;
	float		refractive_index;
	float		emission_exp;
	float		brightness;
	t_uint32	normal_map;
	t_uint32	bump_map;
	float		density;
};
#endif
