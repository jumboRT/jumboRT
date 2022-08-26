#ifndef CL_H
# define CL_H

# include "rtmath.h"

# ifndef RT_OPENCL

#  include <stdint.h>

#  define GLOBAL

# else

typedef signed char		int8_t;
typedef signed short	int16_t;
typedef signed int		int32_t;
typedef signed long		int64_t;
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;
typedef unsigned int	uint32_t;
typedef unsigned long	uint64_t;

# endif

typedef uint64_t			t_seed;

t_seed	rt_random(GLOBAL t_seed *seed);
float	rt_random_float(GLOBAL t_seed *seed);
float	rt_random_float_range(GLOBAL t_seed *seed, float min, float max);
t_vec	rt_random_in_sphere(GLOBAL t_seed *seed);
t_vec	rt_random_on_hemi(GLOBAL t_seed *seed, t_vec normal);

#endif
