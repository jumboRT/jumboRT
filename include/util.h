#ifndef UTIL_H
# define UTIL_H

# include "rtmath.h"
# include <stddef.h>

# ifndef RT_BUFFER_SIZE
#  define RT_BUFFER_SIZE 10240
# elif RT_BUFFER_SIZE <= 0
#  error RT_BUFFER_SIZE must be a positive integer
# endif

typedef struct s_thread_ctx	t_thread_ctx;
typedef unsigned int		t_color;
typedef unsigned long		t_seed;

struct s_rt_state;

/* TODO: count use a bit vector for ctx->visited */

struct s_thread_ctx {
    t_seed				seed;
	unsigned char		*visited;
	struct s_rt_state	*state;
};

void	rt_assert(int condition, const char *msg);
void	*rt_malloc(size_t size);
void	*rt_realloc(void *ptr, size_t old_size, size_t new_size);
void	rt_free(void *ptr);
void	*rt_memdup(const void *src, size_t size);

int		rt_atof(const char *str, FLOAT *dst);
char	*rt_readfile(const char *path, char **error, size_t *size);

t_seed	rt_random(t_seed *seed);
FLOAT	rt_random_float(t_seed *seed);
void	rt_random_range(t_seed *seed, size_t *dst, size_t n);
FLOAT	rt_random_float_range(t_seed *seed, FLOAT min, FLOAT max);
t_vec	rt_random_hvec(t_seed *seed, t_vec dir);
t_vec	rt_random_svec(t_seed *seed);


#endif
