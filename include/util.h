/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   util.h                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:53 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:53 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTIL_H
# define UTIL_H

# include <stddef.h>
# include <unistd.h>
# include <stdint.h>

# if RT_BONUS
#  ifdef RT_WINDOWS
#   include <winsock2.h>
#   include <winsock.h>
#  else
#   include <sys/socket.h>
#   include <poll.h>
#  endif
# endif

# ifndef RT_BUFFER_SIZE
#  define RT_BUFFER_SIZE 262144
# elif RT_BUFFER_SIZE <= 0
#  error RT_BUFFER_SIZE must be a positive integer
# endif

typedef struct s_image	t_image;

size_t	rt_image_to_ppm(char **dest, const t_image *image);
size_t	rt_image_to_pfm(char **dest, const t_image *image);

int		rt_atof(const char *str, float *dst);
int		rt_atol(const char *str, unsigned long *dst);

void	*rt_malloc(size_t size) __attribute__ ((malloc));
void	*rt_realloc(void *ptr, size_t old_size, size_t new_size);
void	*rt_reallog(void *ptr, size_t *capacity, size_t size);
void	rt_free(void *ptr);
void	rt_free4(void *a, void *b, void *c, void *d);

void	rt_assert(int condition, const char *msg);
void	*rt_memdup(const void *src, size_t size);
void	*rt_memcpy(void *dst, const void *src, size_t size);

size_t	rt_image_to_ppm(char **dest, const t_image *image);
size_t	rt_write_ppm(const char *path, const t_image *image);
size_t	rt_write_pfm(const char *path, const t_image *image);

char	*rt_readfile(const char *path, char **error, size_t *size);
ssize_t	rt_writefile(const char *path, char **error, const void *data,
			size_t size);

# if RT_BONUS

int		rt_poll(struct pollfd fds[], int nfds, int timeout);
# endif
#endif
