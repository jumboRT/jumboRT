/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   shape.h                                        #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:52 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:52 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHAPE_H
# define SHAPE_H

# include "rtmath.h"
# include "types.h"
# include "aabb.h"

# define RT_SHAPE_TRIANGLE			0
# define RT_SHAPE_SPHERE			1
# define RT_SHAPE_PLANE				2
# define RT_SHAPE_CYLINDER			3
# define RT_SHAPE_CONE				4
# define RT_SHAPE_PARABOLOID		5
# define RT_SHAPE_HYPERBOLOID		6
# define RT_SHAPE_POINT				7

typedef struct s_world				t_world;
typedef struct s_world_hit			t_world_hit;
typedef struct s_context			t_context;

typedef struct s_primitive			t_primitive;
typedef struct s_shape_triangle		t_shape_triangle;
typedef struct s_shape_sphere		t_shape_sphere;
typedef struct s_shape_plane		t_shape_plane;
typedef struct s_shape_cylinder		t_shape_cylinder;
typedef struct s_shape_cone			t_shape_cone;
typedef struct s_shape_paraboloid	t_shape_paraboloid;
typedef struct s_shape_hyperboloid	t_shape_hyperboloid;
typedef struct s_shape_point		t_shape_point;

struct s_primitive {
	t_uint32	data;
};

struct s_shape_triangle {
	t_primitive	base;
	t_uint32	a;
	t_uint32	b;
	t_uint32	c;
};

struct s_shape_sphere {
	t_primitive	base;
	float		radius;
	t_vec		pos;
};

struct s_shape_plane {
	t_primitive	base;
	t_plane		plane;
};

struct s_shape_cylinder {
	t_primitive	base;
	t_cylinder	cylinder;
};

struct s_shape_cone {
	t_primitive	base;
	t_cone		cone;
};

struct s_shape_paraboloid {
	t_primitive		base;
	t_paraboloid	paraboloid;
};

struct s_shape_hyperboloid {
	t_primitive		base;
	t_hyperboloid	hyperboloid;
};

struct s_shape_point {
	t_primitive	base;
	t_vec		pos;
};

t_uint32	prim_type(const GLOBAL t_primitive *prim);
t_bounds	prim_bounds(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world);
int			prim_intersect(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_ray_min ray,
				t_world_hit *hit);
void		prim_hit_info(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, t_ray ray,
				t_world_hit *hit);
t_vec		prim_sample(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world, GLOBAL t_context *ctx);
float		prim_area(const GLOBAL t_primitive *prim,
				const GLOBAL t_world *world);
int			prim_is_infinite(const GLOBAL t_primitive *prim);
int			prim_is_degenerate(const GLOBAL t_primitive *prim);

#endif
