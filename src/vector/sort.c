#include "vector.h"

static void
	vector_swap(t_vector *vec, size_t a, size_t b)
{
	unsigned char	*a_ptr;
	unsigned char	*b_ptr;
	size_t			i;

	if (a == b)
		return ;
	a_ptr = vector_at(vec, a);
	b_ptr = vector_at(vec, b);
	i = 0;
	while (i < vec->elem_size)
	{
		a_ptr[i] ^= b_ptr[i];
		b_ptr[i] ^= a_ptr[i];
		a_ptr[i] ^= b_ptr[i];
		i += 1;
	}
}

static t_range
	vector_partition(t_vector *vec, t_range range, t_compare compare, void *ctx)
{
	static t_seed	seed = 7549087012;
	size_t			index;
	t_range			mid;
	int				cmp;

	index = rt_random(&seed) % (range.max - range.min);
	vector_swap(vec, range.min, range.min + index);
	mid.min = range.min;
	range.min += 1;
	mid.max = range.min;
	while (range.min < range.max)
	{
		cmp = compare(vector_at(vec, range.min), vector_at(vec, mid.min), ctx);
		if (cmp < 0)
		{
			vector_swap(vec, range.min, mid.min);
			mid.min += 1;
		}
		if (cmp <= 0)
		{
			vector_swap(vec, range.min, mid.max);
			mid.max += 1;
		}
		range.min += 1;
	}
	return (mid);
}

static void
	vector_quicksort(t_vector *vec, t_range range, t_compare compare, void *ctx)
{
	t_range	mid;
	t_range	tmp;

	if (range.min + 1 < range.max)
	{
		mid = vector_partition(vec, range, compare, ctx);
		tmp.min = range.min;
		tmp.max = mid.min;
		vector_quicksort(vec, tmp, compare, ctx);
		tmp.min = mid.max;
		tmp.max = range.max;
		vector_quicksort(vec, tmp, compare, ctx);
	}
}

void
	vector_sort(t_vector *vec, t_compare compare, void *ctx)
{
	t_range	range;

	range.min = 0;
	range.max = vector_size(vec);
	vector_quicksort(vec, range, compare, ctx);
}

