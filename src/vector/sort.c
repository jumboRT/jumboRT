#include "vector.h"

static size_t
	vector_sort_median(t_vector *vec, t_range range, t_compare compare, void *ctx)
{
	size_t	lo;
	size_t	hi;
	size_t	mi;

	lo = range.min;
	hi = range.max - 1;
	mi = (lo + hi) / 2;
	if (compare(vector_at(vec, lo), vector_at(vec, mi), ctx) > 0)
		vector_swap(vec, lo, mi);
	if (compare(vector_at(vec, lo), vector_at(vec, hi), ctx) > 0)
		vector_swap(vec, lo, hi);
	if (compare(vector_at(vec, mi), vector_at(vec, hi), ctx) > 0)
		vector_swap(vec, mi, hi);
	return (mi);
}

static size_t
	vector_partition(t_vector *vec, t_range range, t_compare compare, void *ctx)
{
	size_t	p;
	size_t	i;
	size_t	j;
	void	*pivot;

	p = vector_sort_median(vec, range, compare, ctx);
	vector_swap(vec, range.min, p);
	i = range.min - 1;
	j = range.max;
	while (1)
	{
		pivot = vector_at(vec, p);
		while (compare(vector_at(vec, ++i), pivot, ctx) < 0)
			continue ;
		while (compare(vector_at(vec, --j), pivot, ctx) > 0)
			continue ;
		if (i >= j)
			return (j + 1);
		vector_swap(vec, i, j);
		if (i == p)
			p = j;
		else if (j == p)
			p = i;
	}
}

static void
	vector_quicksort(t_vector *vec, t_range range, t_compare compare, void *ctx)
{
	size_t	mid;
	t_range	tmp;

	if (range.min + 2 == range.max)
	{
		if (compare(vector_at(vec, range.min), vector_at(vec, range.min + 1), ctx) > 0)
			vector_swap(vec, range.min, range.min + 1);
	}
	else if (range.min + 3 == range.max)
	{
		vector_sort_median(vec, range, compare, ctx);
	}
	else if (range.min + 1 < range.max)
	{
		mid = vector_partition(vec, range, compare, ctx);
		tmp.min = range.min;
		tmp.max = mid;
		vector_quicksort(vec, tmp, compare, ctx);
		tmp.min = mid;
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

