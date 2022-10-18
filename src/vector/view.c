#include "vector.h"

t_view
	view_view(t_view view, size_t index, size_t size)
{
	t_view	result;

	result.data = view_get(view, index);
	result.size = size * view.elem_size;
	result.elem_size = view.elem_size;
	return (result);
}

size_t
	view_size(t_view view)
{
	return (view.size / view.elem_size);
}

void
	*view_get(t_view view, size_t index)
{
	return ((char *) view.data + view.elem_size * index);
}

void
	view_each(t_view view, t_function func)
{
	size_t	i;

	if (func == NULL)
		return ;
	i = 0;
	while (i < view_size(view))
	{
		func(view_get(view, i));
		i += 1;
	}
}
