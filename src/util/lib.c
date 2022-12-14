/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   lib.c                                          #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:17 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:17 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#ifdef RT_USE_LIBC
# include <string.h>
#else
# include <libft.h>
#endif

#ifdef RT_USE_LIBC

void
	*rt_memcpy(void *dst, const void *src, size_t size)
{
	return (memcpy(dst, src, size));
}

#else

void
	*rt_memcpy(void *dst, const void *src, size_t size)
{
	return (ft_memcpy(dst, src, size));
}

#endif
