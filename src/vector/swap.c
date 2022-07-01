#include "vector.h"

#include "world_impl.h"

static void
	swap_int(t_vector *vec, size_t a, size_t b, size_t size)
{
	unsigned char	*a_ptr;
	unsigned char	*b_ptr;
	unsigned char	tmp;
	size_t			i;

	a_ptr = vector_at(vec, a);
	b_ptr = vector_at(vec, b);
	i = 0;
	while (i < size)
	{
		tmp = a_ptr[i];
		a_ptr[i] = b_ptr[i];
		b_ptr[i] = tmp;
		i += 1;
	}
}

void
	vector_swap(t_vector *vec, size_t a, size_t b)
{
	if (vec->elem_size == 4)
		swap_int(vec, a, b, 4);
	else if (vec->elem_size == sizeof(t_edge))
		swap_int(vec, a, b, sizeof(t_edge));
	else
		swap_int(vec, a, b, vec->elem_size);
}

