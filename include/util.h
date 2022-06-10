#ifndef UTIL_H
# define UTIL_H

# include <stddef.h>

void	*rt_malloc(size_t size) __attribute__ ((malloc));
void	*rt_realloc(void *ptr, size_t old_size, size_t new_size) __attribute__ ((malloc));
void	*rt_reallog(void *ptr, size_t *capacity, size_t size);
void	rt_free(void *ptr);

void	rt_assert(int condition, const char *msg) __attribute__ ((pure));
void	*rt_memdup(const void *src, size_t size) __attribute__ ((pure));

#endif
