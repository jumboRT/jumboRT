#include "vector.h"

#include "accel_impl.h"

static void
	swap(unsigned char *a, unsigned char *b, size_t size)
{
	unsigned char	tmp;
	size_t			i;

	i = 0;
	while (i < size)
	{
		tmp = a[i];
		a[i] = b[i];
		b[i] = tmp;
		i += 1;
	}
}

void
	view_swap(t_view view, size_t a, size_t b)
{
	if (view.elem_size == 4)
		swap(view_get(view, a), view_get(view, b), 4);
	else if (view.elem_size == sizeof(t_edge))
		swap(view_get(view, a), view_get(view, b), sizeof(t_edge));
	else
		swap(view_get(view, a), view_get(view, b), view.elem_size);
}
