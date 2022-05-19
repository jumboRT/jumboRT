#include "util.h"
#include <libft.h>

void
	*rt_memdup(const void *src, size_t size)
{
	void	*cpy;

	cpy = rt_malloc(size);
	ft_memcpy(cpy, src, size);
	return (cpy);
}
