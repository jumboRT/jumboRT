#include "util.h"

unsigned int
	rt_hash(const char *str)
{
	unsigned int	basis;	
	size_t			index;

	index = 0;
	basis = 2166136261;
	while (str[index] != '\0')
	{
		basis = (basis ^ str[index]) * 16777619;
		++index;
	}
	return (basis);
}
