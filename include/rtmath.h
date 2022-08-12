#ifndef RTMATH_H
# define RTMATH_H

# define RT_TINY_VAL 0.0001
# define RT_HUGE_VAL 1000000000.0

# define RT_1_PI 0.31831 
# define RT_PI 3.14159
# define RT_2PI 6.28319
# if !defined FLOAT
#  define FLOAT float
# endif

# define RT_VECTORIZE

# if defined RT_VECTORIZE
typedef FLOAT				t_vec __attribute__ ((vector_size(16)));
typedef int					t_ivec __attribute__ ((vector_size(16))); /* TODO can probably be removed */
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
	t_vec	normals[3];
	int		is_smooth;
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
	t_vec	shading_normal;
	t_vec	geometric_normal;
	t_vec	dpdu;
	t_vec	dpdv;
	t_vec2	uv;
	FLOAT	t;
};

FLOAT		rt_sqrt(FLOAT a);
FLOAT		rt_cbrt(FLOAT a);
FLOAT		rt_sin(FLOAT a);
FLOAT		rt_asin(FLOAT a);
FLOAT		rt_cos(FLOAT a);
FLOAT		rt_acos(FLOAT a);
FLOAT		rt_tan(FLOAT a);
FLOAT		rt_atan(FLOAT a);
FLOAT		rt_atan2(FLOAT y, FLOAT x);
FLOAT		rt_abs(FLOAT a);
FLOAT		rt_min(FLOAT a, FLOAT b);
FLOAT		rt_max(FLOAT a, FLOAT b);
FLOAT		rt_pow(FLOAT x, FLOAT y);
FLOAT		rt_mod(FLOAT a, FLOAT b);
FLOAT		rt_exp(FLOAT arg);
FLOAT		rt_clamp(FLOAT arg, FLOAT min, FLOAT max);
FLOAT		rt_log(FLOAT arg);

int			float_eq(FLOAT a, FLOAT b, FLOAT error);

FLOAT		x(t_vec v);
FLOAT		y(t_vec v);
FLOAT		z(t_vec v);
FLOAT		w(t_vec v);
FLOAT		xyz(t_vec v, int i);

t_vec		vec_set(t_vec v, int axis, FLOAT val);

t_vec		vec(FLOAT x, FLOAT y, FLOAT z, FLOAT w);
t_vec		vec3(FLOAT x, FLOAT y, FLOAT z);
t_vec		vec_0(void);
t_vec		vec_x(FLOAT x);
t_vec		vec_y(FLOAT y);
t_vec		vec_z(FLOAT z);

t_vec2		vec2(FLOAT x, FLOAT y);
t_vec		vec2_0(void);
t_vec		vec2_x(FLOAT x);
t_vec		vec2_y(FLOAT y);

FLOAT		u(t_vec2 v);
FLOAT		v(t_vec2 v);

t_plane		plane(t_vec pos, t_vec normal);
t_triangle	triangle(t_vec v0, t_vec v1, t_vec v2, t_vec2 uv0, t_vec2 uv1, t_vec2 uv2, t_vec n1, t_vec n2, t_vec n3, int is_smooth);
t_sphere	sphere(t_vec pos, FLOAT radius);
t_cylinder	cylinder(t_vec pos, t_vec dir, FLOAT height, FLOAT radius);
t_cone		cone(t_vec pos, t_vec dir, FLOAT height, FLOAT angle); 

FLOAT		vec_dot(t_vec a, t_vec b);
FLOAT		vec_mag2(t_vec a);
FLOAT		vec_mag(t_vec a);

t_vec		vec_neg(t_vec v);
t_vec		vec_scale(t_vec v, FLOAT s);
t_vec		vec_cross(t_vec a, t_vec b);
t_vec		vec_add(t_vec a, t_vec b);
t_vec		vec_sub(t_vec a, t_vec b);
t_vec		vec_mul(t_vec a, t_vec b);
t_vec		vec_norm(t_vec v);

t_vec2		vec2_neg(t_vec2 v);
t_vec2		vec2_scale(t_vec2 v, FLOAT s);
t_vec2		vec2_add(t_vec2 a, t_vec2 b);
t_vec2		vec2_sub(t_vec2 a, t_vec2 b);
t_vec2		vec2_mul(t_vec2 a, t_vec2 b);
/* Safe version of vec_norm that doesn't divide by 0 */
t_vec		vec_norm2(t_vec v);

t_vec		vec_min(t_vec a, t_vec b);
t_vec		vec_max(t_vec a, t_vec b);
t_vec		vec_clamp(t_vec v, FLOAT min, FLOAT max);
t_vec		vec_abs(t_vec a);

t_vec		vec_tangent(t_vec v);
t_vec		vec_rotate(t_vec axis, t_vec v, FLOAT angle);
void		vec_angles(t_vec basis, t_vec v, t_vec *out_vec, FLOAT *out_angle);

t_vec2		vec_change_basis2(t_vec v, t_vec tangent, t_vec bit_tangent);

int			vec_eq(t_vec a, t_vec b);

t_ray		ray(t_vec org, t_vec dir);
t_vec		ray_at(t_ray ray, FLOAT t);

t_vec2		sphere_uv_at(t_vec point);

int			ray_plane_intersect(t_ray ray,
				t_plane plane, FLOAT min, t_hit *hit);
int			ray_triangle_intersect(t_ray ray,
				t_triangle triangle, FLOAT min, t_hit *hit);
int			ray_sphere_intersect(t_ray ray,
				t_sphere sphere, FLOAT min, t_hit *hit);
int			ray_cylinder_intersect(t_ray ray,
				t_cylinder cylinder, FLOAT min, t_hit *hit);
int			ray_cone_intersect(t_ray ray,
				t_cone cone, FLOAT min, t_hit *hit);

void		plane_hit_info(t_ray ray,
				t_plane plane, t_hit *hit);
void		triangle_hit_info(t_ray ray,
				t_triangle triangle, t_hit *hit);
void		sphere_hit_info(t_ray ray,
				t_sphere sphere, t_hit *hit);
void		cylinder_hit_info(t_ray ray,
				t_cylinder cylinder, t_hit *hit);
void		cone_hit_info(t_ray ray,
				t_cone cone, t_hit *hit);

int			quadratic_solve(const t_quadratic *quadratic, FLOAT solutions[2]);
#endif
