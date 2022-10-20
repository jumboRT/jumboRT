#include "util.h"

#include <stdlib.h>
#include <libft.h>

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
		rt_memcpy(ret, ptr, size);
		rt_free(ptr);
	}
	return (ret);
}

void
	*rt_reallog(void *ptr, size_t *capacity, size_t size)
{
	size_t	old_capacity;
	size_t	new_capacity;

	old_capacity = *capacity;
	if (old_capacity >= size)
		return (ptr);
	new_capacity = old_capacity * 2;
	if (new_capacity < size)
		new_capacity = size;
	*capacity = new_capacity;
	return (rt_realloc(ptr, old_capacity, new_capacity));
}

void
	rt_free(void *ptr)
{
	free(ptr);
}

void
	rt_free4(void *a, void *b, void *c, void *d)
{
	rt_free(a);
	rt_free(b);
	rt_free(c);
	rt_free(d);
}
