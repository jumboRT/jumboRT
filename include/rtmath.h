#ifndef RTMATH_H
# define RTMATH_H

# define RT_TINY_VAL 0.001
# define RT_HUGE_VAL 1000000000.0

# define RT_E 0.57721
# define RT_1_PI 0.31831 
# define RT_PI 3.14159
# define RT_2PI 6.28319
# define RT_PI_2 1.57080
# define RT_PI_4 0.78540

# if defined RT_VECTORIZE
typedef float				t_vec __attribute__ ((vector_size(16)));
typedef float				t_vec2 __attribute__ ((vector_size(8)));

typedef union u_vec_conv {
	t_vec	vec;
	float	elem[4];
}	t_vec_conv;

typedef union u_vec2_conv {
	t_vec2	vec;
	float	elem[2];
}	t_vec2_conv;
# endif

typedef struct s_ray		t_ray;

typedef struct s_plane			t_plane;
typedef struct s_triangle		t_triangle;
typedef struct s_sphere			t_sphere;
typedef struct s_cylinder		t_cylinder;
typedef struct s_cone			t_cone;
typedef struct s_paraboloid		t_paraboloid;
typedef struct s_hyperboloid	t_hyperboloid;

typedef struct s_hit		t_hit;

typedef struct s_quadratic	t_quadratic;

# if !defined RT_VECTORIZE
typedef struct s_vec {
	float	x;
	float	y;
	float	z;
	float	w;
}	t_vec;

typedef struct s_vec2 {
	float	x;
	float	y;
}	t_vec2;
# endif

struct s_ray {
	t_vec	org;
	t_vec	dir;
};

# if defined RT_VECTORIZE
struct __attribute__((aligned(16))) s_quadratic {
	float	a;
	float	b;
	float	c;
};
#else
struct s_quadratic {
	float	a;
	float	b;
	float	c;
};
#endif

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
	float	radius;
};

struct s_cylinder {
	t_vec	pos;
	t_vec	dir;
	float	height;
	float	radius;
};

struct s_cone {
	t_vec	pos;
	t_vec	dir;
	float	height;
	float	angle;
};

struct s_paraboloid {
	t_vec	pos;
	float	a;
	float	b;
};

struct s_hyperboloid {
	t_vec	pos;
	float	a;
	float	b;
	float	c;
};

struct s_hit {
	t_vec	pos;
	t_vec	shading_normal;
	t_vec	geometric_normal;
	t_vec	dpdu;
	t_vec	dpdv;
	t_vec2	uv;
	float	t;
	union {
		struct {
			float	bc_u;
			float	bc_v;
			float	bc_w;
		} tr;
	} ctx;
};

float		rt_sqrt(float a);
float		rt_cbrt(float a);
float		rt_sin(float a);
float		rt_asin(float a);
float		rt_cos(float a);
float		rt_acos(float a);
float		rt_tan(float a);
float		rt_atan(float a);
float		rt_atan2(float y, float x);
float		rt_abs(float a);
float		rt_min(float a, float b);
float		rt_max(float a, float b);
float		rt_pow(float x, float y);
float		rt_mod(float a, float b);
float		rt_exp(float arg);
float		rt_clamp(float arg, float min, float max);
float		rt_log(float arg);
float		rt_degtorad(float rad);

float		rt_gc(float arg);
float		rt_igc(float arg);
t_vec		vec_gc(t_vec v);
t_vec		vec_igc(t_vec v);

float		x(t_vec v);
float		y(t_vec v);
float		z(t_vec v);
float		w(t_vec v);
float		xyz(t_vec v, int i);

t_vec		vec_set(t_vec v, int axis, float val);

t_vec		vec(float x, float y, float z, float w);
t_vec		vec3(float x, float y, float z);
t_vec		vec_0(void);
t_vec		vec_x(float x);
t_vec		vec_y(float y);
t_vec		vec_z(float z);

t_vec2		vec2(float x, float y);
t_vec2		vec2_0(void);
t_vec		vec2_x(float x);
t_vec		vec2_y(float y);

float		u(t_vec2 v);
float		v(t_vec2 v);

t_plane			plane(t_vec pos, t_vec normal);
t_triangle		triangle(t_vec v0, t_vec v1, t_vec v2, t_vec2 uv0, t_vec2 uv1, t_vec2 uv2, t_vec n1, t_vec n2, t_vec n3, int is_smooth);
t_sphere		sphere(t_vec pos, float radius);
t_cylinder		cylinder(t_vec pos, t_vec dir, float height, float radius);
t_cone			cone(t_vec pos, t_vec dir, float height, float angle); 
t_paraboloid	paraboloid(t_vec pos, float a, float b);
t_hyperboloid	hyperboloid(t_vec pos, float a, float b, float c);

float		vec_dot(t_vec a, t_vec b);
float		vec_mag2(t_vec a);
float		vec_mag(t_vec a);

t_vec		vec_neg(t_vec v);
t_vec		vec_scale(t_vec v, float s);
t_vec		vec_cross(t_vec a, t_vec b);
t_vec		vec_add(t_vec a, t_vec b);
t_vec		vec_sub(t_vec a, t_vec b);
t_vec		vec_mul(t_vec a, t_vec b);
t_vec		vec_norm(t_vec v);

t_vec2		vec2_neg(t_vec2 v);
t_vec2		vec2_scale(t_vec2 v, float s);
t_vec2		vec2_add(t_vec2 a, t_vec2 b);
t_vec2		vec2_sub(t_vec2 a, t_vec2 b);
t_vec2		vec2_mul(t_vec2 a, t_vec2 b);
/* Safe version of vec_norm that doesn't divide by 0 */
t_vec		vec_norm2(t_vec v);

t_vec		vec_min(t_vec a, t_vec b);
t_vec		vec_max(t_vec a, t_vec b);
float		vec_minv(t_vec v);
float		vec_maxv(t_vec v);
float		vec_xyz_maxv(t_vec v);
t_vec		vec_clamp(t_vec v, float min, float max);
t_vec		vec_abs(t_vec a);

t_vec		vec_tangent(t_vec v);
t_vec		vec_rotate(t_vec axis, t_vec v, float angle);
void		vec_angles(t_vec basis, t_vec v, t_vec *out_vec, float *out_angle);

t_vec2		vec_change_basis2(t_vec v, t_vec tangent, t_vec bit_tangent);

int			vec_eq(t_vec a, t_vec b);
int			vec2_eq(t_vec2 a, t_vec2 b);

t_ray		ray(t_vec org, t_vec dir);
t_vec		ray_at(t_ray ray, float t);

t_vec2		sphere_uv_at(t_vec point);
t_vec		sphere_to_cart(float theta, float phi);

int			ray_plane_intersect(t_ray ray,
				t_plane plane, float min, t_hit *hit);
int			ray_triangle_intersect(t_ray ray,
				t_triangle triangle, float min, t_hit *hit);
int			ray_sphere_intersect(t_ray ray,
				t_sphere sphere, float min, t_hit *hit);
int			ray_cylinder_intersect(t_ray ray,
				t_cylinder cylinder, float min, t_hit *hit);
int			ray_cone_intersect(t_ray ray,
				t_cone cone, float min, t_hit *hit);
int			ray_paraboloid_intersect(t_ray ray,
				t_paraboloid paraboloid, float min, t_hit *hit);
int			ray_hyperboloid_intersect(t_ray ray,
				t_hyperboloid hyperboloid, float min, t_hit *hit);

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
void		paraboloid_hit_info(t_ray ray,
				t_paraboloid paraboloid, t_hit *hit);
void		hyperboloid_hit_info(t_ray ray,
				t_hyperboloid hyperboloid, t_hit *hit);

int			quadratic_solve(const t_quadratic *quadratic, float solutions[2]);
#endif
