#include "vector.h"

static size_t
	median(t_view view, t_compare cmp)
{
	const size_t	bot = 0;
	const size_t	top = view_size(view) - 1;
	const size_t	mid = (bot + top) / 2;

	if (cmp(view_get(view, bot), view_get(view, mid)) > 0)
		view_swap(view, bot, mid);
	if (cmp(view_get(view, bot), view_get(view, top)) > 0)
		view_swap(view, bot, top);
	if (cmp(view_get(view, mid), view_get(view, top)) > 0)
		view_swap(view, mid, top);
	return (mid);
}

static size_t
	partition(t_view view, t_compare cmp)
{
	size_t	p;
	size_t	i;
	size_t	j;
	void	*pivot;

	p = median(view, cmp);
	view_swap(view, 0, p);
	i = (size_t) 0 - 1;
	j = view_size(view);
	while (1)
	{
		pivot = view_get(view, p);
		while (cmp(view_get(view, ++i), pivot) < 0)
			continue ;
		while (cmp(view_get(view, --j), pivot) > 0)
			continue ;
		if (i >= j)
			return (j + 1);
		view_swap(view, i, j);
		if (i == p)
			p = j;
		else if (j == p)
			p = i;
	}
}

void
	view_sort(t_view view, t_compare cmp)
{
	size_t	mid;
	size_t	end;

	if (view_size(view) == 2)
	{
		if (cmp(view_get(view, 0), view_get(view, 1)) > 0)
			view_swap(view, 0, 1);
	}
	else if (view_size(view) == 3)
	{
		if (cmp(view_get(view, 0), view_get(view, 1)) > 0)
			view_swap(view, 0, 1);
		if (cmp(view_get(view, 0), view_get(view, 2)) > 0)
			view_swap(view, 0, 2);
		if (cmp(view_get(view, 1), view_get(view, 2)) > 0)
			view_swap(view, 1, 2);
	}
	else if (view_size(view) >= 4)
	{
		mid = partition(view, cmp);
		view_sort(view_view(view, 0, mid), cmp);
		end = view_size(view) - mid;
		view_sort(view_view(view, mid, end), cmp);
	}
}

