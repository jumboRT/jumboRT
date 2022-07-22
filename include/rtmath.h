#ifndef RTMATH_H
# define RTMATH_H

# define RT_HUGE_VAL 1000000000.0

# define RT_PI 3.14159
# define RT_2PI 6.28319
# if !defined FLOAT
#  define FLOAT float
# endif

# define RT_VECTORIZE

# if defined RT_VECTORIZE
typedef FLOAT				t_vec __attribute__ ((vector_size(16)));
typedef FLOAT				t_vec2 __attribute__ ((vector_size(8)));

typedef union u_vec_conv {
	t_vec	vec;
	FLOAT	elem[4];
}	t_vec_conv;

typedef union u_vec2_conv {
	t_vec2	vec;
	FLOAT	elem[2];
}	t_vec2_conv;
# endif

typedef struct s_ray		t_ray;

typedef struct s_plane		t_plane;
typedef struct s_triangle	t_triangle;
typedef struct s_sphere		t_sphere;
typedef struct s_cylinder	t_cylinder;
typedef struct s_cone		t_cone;

typedef struct s_hit		t_hit;

typedef struct s_quadratic	t_quadratic;

/* TODO: make all the attributes conditional */

# if !defined RT_VECTORIZE
typedef struct __attribute__((aligned(16))) s_vec {
	FLOAT	x;
	FLOAT	y;
	FLOAT	z;
	FLOAT	w;
}	t_vec;

typedef struct s_vec2 {
	FLOAT	x;
	FLOAT	y;
}	t_vec2;
# endif

struct s_ray {
	t_vec	org;
	t_vec	dir;
};

struct __attribute__((aligned(16))) s_quadratic {
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
	t_vec2	uvs[3];
};

struct s_sphere {
	t_vec	pos;
	FLOAT	radius;
};

struct s_cylinder {
	t_vec	pos;
	t_vec	dir;
	FLOAT	height;
	FLOAT	radius;
};

struct s_cone {
	t_vec	pos;
	t_vec	dir;
	FLOAT	height;
	FLOAT	angle;
};

struct s_hit {
	t_vec	pos;
	t_vec	normal;
	t_vec2	uv;
	FLOAT	t;
};

FLOAT	rt_sqrt(FLOAT a);
FLOAT	rt_sin(FLOAT a);
FLOAT	rt_asin(FLOAT a);
FLOAT	rt_cos(FLOAT a);
FLOAT	rt_acos(FLOAT a);
FLOAT	rt_tan(FLOAT a);
FLOAT	rt_atan(FLOAT a);
FLOAT	rt_atan2(FLOAT y, FLOAT x);
FLOAT	rt_abs(FLOAT a);
FLOAT	rt_min(FLOAT a, FLOAT b);
FLOAT	rt_max(FLOAT a, FLOAT b);
FLOAT	rt_pow(FLOAT x, FLOAT y);

int float_eq(FLOAT a, FLOAT b, FLOAT error) __attribute__ ((const));

FLOAT	x(t_vec v) __attribute__ ((const));
FLOAT	y(t_vec v) __attribute__ ((const));
FLOAT	z(t_vec v) __attribute__ ((const));
FLOAT	w(t_vec v) __attribute__ ((const));
FLOAT	xyz(t_vec v, int i) __attribute__ ((const));
t_vec	vec_set(t_vec v, int axis, FLOAT val) __attribute__ ((const));

t_vec	vec(FLOAT x, FLOAT y, FLOAT z)	__attribute__ ((const));
t_vec	vec_0(void) __attribute__ ((const));
t_vec	vec_x(FLOAT x) __attribute__ ((const));
t_vec	vec_y(FLOAT y) __attribute__ ((const));
t_vec	vec_z(FLOAT z) __attribute__ ((const));

t_vec2	vec2(FLOAT x, FLOAT y) __attribute__ ((const));
t_vec	vec2_0(void) __attribute__ ((const));
t_vec	vec2_x(FLOAT x) __attribute__ ((const));
t_vec	vec2_y(FLOAT y) __attribute__ ((const));

FLOAT	u(t_vec2 v) __attribute__ ((const));
FLOAT	v(t_vec2 v) __attribute__ ((const));

t_plane		plane(t_vec pos, t_vec normal) __attribute__ ((const));
t_triangle	triangle(t_vec v0, t_vec v1, t_vec v2, t_vec2 uv0, t_vec2 uv1, t_vec2 uv2) __attribute__ ((const));
t_sphere	sphere(t_vec pos, FLOAT radius) __attribute__ ((const));
t_cylinder	cylinder(t_vec pos, t_vec dir, FLOAT height, FLOAT radius) __attribute__ ((const));
t_cone		cone(t_vec pos, t_vec dir, FLOAT height, FLOAT angle) __attribute__ ((const)); 

FLOAT	vec_dot(t_vec a, t_vec b) __attribute__ ((const));
FLOAT	vec_mag2(t_vec a) __attribute__ ((const));
FLOAT	vec_mag(t_vec a) __attribute__ ((const));

t_vec	vec_neg(t_vec v) __attribute__ ((const));
t_vec	vec_scale(t_vec v, FLOAT s) __attribute__ ((const));
t_vec	vec_cross(t_vec a, t_vec b) __attribute__ ((const));
t_vec	vec_add(t_vec a, t_vec b) __attribute__ ((const));
t_vec	vec_sub(t_vec a, t_vec b) __attribute__ ((const));
t_vec	vec_mul(t_vec a, t_vec b) __attribute__ ((const));
t_vec	vec_norm(t_vec v) __attribute__ ((const));
/* Safe version of vec_norm that doesn't divide by 0 */
t_vec	vec_norm2(t_vec v) __attribute__ ((const));

t_vec	vec_min(t_vec a, t_vec b) __attribute__ ((const));
t_vec	vec_max(t_vec a, t_vec b) __attribute__ ((const));
t_vec	vec_clamp(t_vec v, FLOAT min, FLOAT max) __attribute__ ((const));

t_vec	vec_rotate(t_vec axis, t_vec v, FLOAT angle);
void	vec_angles(t_vec basis, t_vec v, t_vec *out_vec, FLOAT *out_angle);

int	vec_eq(t_vec a, t_vec b) __attribute__ ((const));

t_ray	ray(t_vec org, t_vec dir) __attribute__ ((const));
t_vec	ray_at(t_ray ray, FLOAT t) __attribute__ ((const));

int	ray_plane_intersect(t_ray ray,
		t_plane plane, FLOAT min, t_hit *hit);
int	ray_triangle_intersect(t_ray ray,
		t_triangle triangle, FLOAT min, t_hit *hit);
int	ray_sphere_intersect(t_ray ray,
		t_sphere sphere, FLOAT min, t_hit *hit);
int	ray_cylinder_intersect(t_ray ray,
		t_cylinder cylinder, FLOAT min, t_hit *hit);
int	ray_cone_intersect(t_ray ray,
		t_cone cone, FLOAT min, t_hit *hit);

int	quadratic_solve(const t_quadratic *quadratic, FLOAT solutions[2]);
#endif
