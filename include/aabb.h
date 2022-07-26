#ifndef AABB_H
# define AABB_H

# include "rtmath.h"

typedef struct s_bounds	t_bounds;

struct s_bounds {
	t_vec	min;
	t_vec	max;
};

t_bounds	bounds(t_vec min, t_vec max);
t_bounds	bounds_0(void);
t_bounds	bounds_max(t_bounds a, t_bounds b);
FLOAT		bounds_surf(t_bounds a);

#endif
