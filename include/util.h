#ifndef UTIL_H
# define UTIL_H

# include "rtmath.h"

# include <stddef.h>
# include <unistd.h>
# include <stdint.h>

# ifndef RT_BUFFER_SIZE
#  define RT_BUFFER_SIZE 262144
# elif RT_BUFFER_SIZE <= 0
#  error RT_BUFFER_SIZE must be a positive integer
# endif

typedef struct s_image t_image;

int		rt_atof(const char *str, float *dst);
int		rt_atol(const char *str, unsigned long *dst);

void	*rt_malloc(size_t size) __attribute__ ((malloc));
void	*rt_realloc(void *ptr, size_t old_size, size_t new_size) __attribute__ ((malloc));
void	*rt_reallog(void *ptr, size_t *capacity, size_t size);
void	rt_free(void *ptr);

void	rt_assert(int condition, const char *msg);
void	*rt_memdup(const void *src, size_t size) __attribute__ ((malloc));
void	*rt_memcpy(void *dst, const void *src, size_t size);

size_t	rt_image_to_ppm(char **dest, const t_image *image);
size_t	rt_write_ppm(const char *path, const t_image *image);
size_t	rt_write_pfm(const char *path, const t_image *image);

char	*rt_readfile(const char *path, char **error, size_t *size);
ssize_t	rt_writefile(const char *path, char **error, const void *data, size_t size);
#endif
