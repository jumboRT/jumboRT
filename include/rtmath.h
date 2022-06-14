#ifndef RTMATH_H
# define RTMATH_H

# define RT_PI 3.14159
# define RT_2PI 6.28319

typedef struct s_vec	t_vec;
typedef struct s_ray	t_ray;

# if !defined FLOAT && defined RT_SSE
#  define FLOAT float
#  include <smmintrin.h>

struct s_vec {
	__m128	v;
};

# else
#  if defined FLOAT
#   error "Only 4 byte floats is currently supported for SSE" 
#  else
#   define FLOAT float
#  endif

struct s_vec {
	FLOAT	x;
	FLOAT	y;
	FLOAT	z;
};

# endif

struct s_ray {
	t_vec	org;
	t_vec	dir;
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

#endif
