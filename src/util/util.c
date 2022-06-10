#include "util.h"

#include <stdlib.h>
#include <unistd.h>
#include <libft.h>
#include <ft_printf.h>

void
	rt_assert(int condition, const char *msg)
{
	if (!condition)
	{
		ft_fprintf(STDERR_FILENO, "assertion failed: %s\n", msg);
		exit(EXIT_FAILURE);
	}
}

void
	*rt_memdup(const void *src, size_t size)
{
	void	*cpy;

	cpy = rt_malloc(size);
	ft_memcpy(cpy, src, size);
	return (cpy);
}

