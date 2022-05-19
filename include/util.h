#ifndef UTIL_H
# define UTIL_H

# include "rtmath.h"
# include <stddef.h>

# ifndef RT_BUFFER_SIZE
#  define RT_BUFFER_SIZE 1024
# elif RT_BUFFER_SIZE <= 0
#  error RT_BUFFER_SIZE must be a positive integer
# endif

typedef unsigned int	t_color;

void			rt_assert(int condition, const char *msg);
void			*rt_malloc(size_t size);
void			*rt_realloc(void *ptr, size_t old_size, size_t new_size);
void			rt_free(void *ptr);
void			*rt_memdup(const void *src, size_t size);

int				rt_atof(const char *str, FLOAT *dst);
char			*rt_readfile(const char *path, char **error);
unsigned long	rt_random(void);
FLOAT			rt_random_float(void);
void			rt_random_range(size_t *dst, size_t n);
FLOAT			rt_random_float_range(FLOAT min, FLOAT max);
t_vec			rt_random_unit_vector(t_vec normal);


#endif
