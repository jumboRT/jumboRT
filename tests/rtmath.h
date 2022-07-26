#ifndef RTMATH_H
# define RTMATH_H

# define RT_PI 3.14159
# define RT_2PI 6.28319
# if !defined FLOAT
#  define FLOAT float
# endif

typedef struct s_vec	t_vec;
typedef struct s_ray	t_ray;

typedef struct s_plane		t_plane;
typedef struct s_triangle	t_triangle;
typedef struct s_sphere		t_sphere;
typedef struct s_cylinder	t_cylinder;
typedef struct s_cone		t_cone;

typedef struct s_hit		t_hit;

typedef struct s_quadratic	t_quadratic;

struct s_vec {
	FLOAT	x;
	FLOAT	y;
	FLOAT	z;
};

struct s_ray {
	t_vec	org;
	t_vec	dir;
};

struct s_quadratic {
	FLOAT	a;
	FLOAT	b;
	FLOAT	c;
};

struct s_plane {
	t_vec	pos;
	t_vec	normal;
};

struct s_triangle {
	t_vec	vertices[3];
	t_vec	normal;
};

struct s_sphere {
	t_vec	pos;
	FLOAT	radius;
};

struct t_cylinder {
	t_vec	pos;
	t_vec	dir;
	FLOAT	height;
	FLOAT	radius;
};

struct t_cone {
	t_vec	pos;
	t_vec	height;
	t_vec	radius;
};

struct	s_hit {
	t_vec	pos;
	t_vec	normal;
	FLOAT	t;
};

int		float_eq(FLOAT a, FLOAT b, FLOAT error) __attribute__ ((const));

FLOAT	x(t_vec v) __attribute__ ((const));
FLOAT	y(t_vec v) __attribute__ ((const));
FLOAT	z(t_vec v) __attribute__ ((const));

t_vec	vec(FLOAT x, FLOAT y, FLOAT z) __attribute__ ((const));
t_vec	vec_0(void) __attribute__ ((const));
t_vec	vec_x(FLOAT x) __attribute__ ((const));
t_vec	vec_y(FLOAT y) __attribute__ ((const));
t_vec	vec_z(FLOAT z) __attribute__ ((const));

t_plane		plane(t_vec pos, t_vec normal)								__attribute__ ((const));
t_triangle	triangle(t_vec v0, t_vec v1, t_vec v2)						__attribute__ ((const));
t_sphere	sphere(t_vec pos, FLOAT radius)								__attribute__ ((const));
t_cylinder	cylinder(t_vec pos, t_vec dir, FLOAT height, FLOAT radius)	__attribute__ ((const));
t_cone		cone(t_vec pos, FLOAT radius, FLOAT height)					__attribute__ ((const));

FLOAT	vec_dot(t_vec a, t_vec b) __attribute__ ((const));
FLOAT	vec_mag2(t_vec a) __attribute__ ((const));
FLOAT	vec_mag(t_vec a) __attribute__ ((const));

t_vec	vec_neg(t_vec v) __attribute__ ((const));
t_vec	vec_scale(t_vec v, FLOAT s) __attribute__ ((const));
t_vec	vec_cross(t_vec a, t_vec b) __attribute__ ((const));
t_vec	vec_add(t_vec a, t_vec b) __attribute__ ((const));
t_vec	vec_sub(t_vec a, t_vec b) __attribute__ ((const));

t_vec	vec_norm(t_vec v) __attribute__ ((const));
/* Safe version of vec_norm that doesn't divide by 0 */
t_vec	vec_norm2(t_vec v) __attribute__ ((const));

t_vec	vec_clamp(t_vec v, FLOAT min, FLOAT max) __attribute__ ((const));

t_ray	ray(t_vec org, t_vec dir) __attribute__ ((const));

t_vec	ray_at(t_ray ray, FLOAT t) __attribute__ ((const));

int		ray_plane_intersect(t_ray ray,
			t_plane plane, FLOAT min, t_hit *hit)		__attribute__ ((const));
int		ray_triange_intersect(t_ray ray,
			t_triangle triangle, FLOAT min, t_hit *hit)	__attribute__ ((const));
int		ray_sphere_intersect(t_ray ray,
			t_sphere sphere, FLOAT min, t_hit *hit)		__attribute__ ((const));
int		ray_cylinder_intersect(t_ray ray,
			t_cylinder cylinder, FLOAT min, t_hit *hit)	__attribute__ ((const));
int		ray_cone_intersect(t_ray ray,
			t_cone cone, FLOAT min, t_hit *hit)			__attribute__ ((const));

int		quadratic_solve(const t_quadratic *quadratic, FLOAT solutions[2]) __attribute__ ((const));
#endif
