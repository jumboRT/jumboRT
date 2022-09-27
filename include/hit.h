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
	t_vec						rel_geometric_normal;
	t_vec						rel_shading_normal;
	const GLOBAL t_primitive	*prim;
	const GLOBAL t_material		*mat;
	int							is_volume;
};

#endif
