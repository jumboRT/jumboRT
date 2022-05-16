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
	*rt_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	rt_assert(ptr != NULL, "malloc failed");
	return (ptr);
}

void
	*rt_realloc(void *ptr, size_t old_size, size_t new_size)
{
	size_t	size;
	void	*ret;

	ret = rt_malloc(new_size);
	if (!ret)
		return (NULL);
	size = old_size;
	if (new_size < old_size)
		size = new_size;
	if (ptr)
	{
		ft_memcpy(ret, ptr, size);
		free(ptr);
	}
	return (ret);
}

void
	rt_free(void *ptr)
{
	free(ptr);
}
