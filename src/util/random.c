#include "util.h"

#include <stddef.h>

unsigned long
	rt_random(void)
{
	static unsigned long	state = 7549087012;

	state ^= state << 12;
	state ^= state >> 25;
	state ^= state << 27;
	return (state * 0x2545F4914F6CDD1DULL);
}

void
	rt_random_range(size_t *dst, size_t n)
{
	size_t	i;
	size_t	j;
	size_t	tmp;
	
	i = 0;
	while (i < n)
	{
		dst[i] = i;
		i += 1;
	}
	i = 0;
	while (i < n - 1)
	{
		j = rt_random() % (n - i - 1) + i + 1;
		tmp = dst[i];
		dst[i] = dst[j];
		dst[j] = tmp;
		i += 1;
	}
}
