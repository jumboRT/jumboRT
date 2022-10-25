/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   util.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:16 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:16 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"

#include <stdlib.h>
#include <unistd.h>
#include <libft.h>
#include <ft_printf.h>
#include <signal.h>

#ifndef RT_DEBUG

void
	rt_assert(int condition, const char *msg)
{
	if (!condition)
	{
		ft_fprintf(STDERR_FILENO, "assertion failed: %s\n", msg);
		exit(EXIT_FAILURE);
	}
}

#else

void
	rt_assert(int condition, const char *msg)
{
	if (!condition)
	{
		ft_fprintf(STDERR_FILENO, "assertion failed: %s\n", msg);
		raise(SIGTRAP);
	}
}

#endif

void
	*rt_memdup(const void *src, size_t size)
{
	void	*cpy;

	cpy = rt_malloc(size);
	rt_memcpy(cpy, src, size);
	return (cpy);
}
