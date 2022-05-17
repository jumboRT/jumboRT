#ifndef UTIL_H
# define UTIL_H

# include <stddef.h>

typedef unsigned int	t_color;

void	rt_assert(int condition, const char *msg);
void	*rt_malloc(size_t size);
void	*rt_realloc(void *ptr, size_t old_size, size_t new_size);
void	rt_free(void *ptr);

int		rt_atof(const char *str, double *dst);

#endif
