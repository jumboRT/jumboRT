#ifndef UTIL_H
# define UTIL_H

# include <stddef.h>
# include <unistd.h>

typedef struct s_image t_image;

void	*rt_malloc(size_t size) __attribute__ ((malloc));
void	*rt_realloc(void *ptr, size_t old_size, size_t new_size) __attribute__ ((malloc));
void	*rt_reallog(void *ptr, size_t *capacity, size_t size);
void	rt_free(void *ptr);

void	rt_assert(int condition, const char *msg);
void	*rt_memdup(const void *src, size_t size) __attribute__ ((malloc));

size_t	rt_image_to_ppm(char **dest, const t_image *image);
size_t	rt_write_ppm(const char *path, const t_image *image);

ssize_t	rt_writefile(const char *path, char **error, const void *data, size_t size);

#endif
