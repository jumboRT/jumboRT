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
	uint32_t	flags;
	t_bsdf		surface;
	t_bsdf		volume;
	t_filter	alpha;
	t_filter	emission;
	float		emission_exp;
	float		brightness;
	uint32_t	normal_map;
	uint32_t	bump_map;
	float		density;
};
#endif
