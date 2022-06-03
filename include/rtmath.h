#ifndef RTMATH_H
# define RTMATH_H

# include <stddef.h>

# define FLOAT double

# define X 0
# define Y 1
# define Z 2
# define W 3

# define RT_PI 3.14159
# define RT_2PI (RT_PI * 2)

typedef struct s_vec	t_vec;
typedef struct s_mat	t_mat;
typedef struct s_ray	t_ray;

struct s_vec {
	FLOAT	v[4];
};

struct s_mat {
	t_vec	rows[4];
};

/* TODO rename pos to org (origin) */
struct s_ray {
	t_vec	pos;
	t_vec	dir;
};

t_vec	vec(FLOAT x, FLOAT y, FLOAT z, FLOAT w);
t_mat	mat(t_vec a, t_vec b, t_vec c, t_vec d);
t_vec	row(t_mat m, size_t i);
t_vec	col(t_mat m, size_t i);

t_mat	diagonal(FLOAT v);
t_mat	transpose(t_mat m);

FLOAT	vec_dot(t_vec a, t_vec b);
FLOAT	vec_mag(t_vec a);
FLOAT	vec_mag2(t_vec a);
t_vec	vec_scale(t_vec a, FLOAT b);

t_vec	vec_cross(t_vec a, t_vec b);
t_vec	vec_add(t_vec a, t_vec b);
t_vec	vec_sub(t_vec a, t_vec b);
t_vec	vec_neg(t_vec a);
t_vec	vec_norm(t_vec a);
t_vec   vec_rotate(t_vec axis, t_vec v, FLOAT angle);
t_vec	vec_clamp(t_vec v, FLOAT min, FLOAT max);

t_vec	vec_hnorm(t_vec a);
t_vec	vec_mul(t_vec a, t_mat b);
t_mat	mat_mul(t_mat a, t_mat b);

t_vec	color_mul(t_vec a, t_vec b);
void	dbg_vec(t_vec a);
void	dbg_mat(t_mat a);

int		float_eq(FLOAT a, FLOAT b, FLOAT error);

t_vec	vec_0(void);
t_vec	vec_x(FLOAT v);
t_vec	vec_y(FLOAT v);
t_vec	vec_z(FLOAT v);
t_vec	vec_w(FLOAT v);

t_vec	vec_min(t_vec a, t_vec b);
t_vec	vec_max(t_vec a, t_vec b);

#endif
