/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vector.c                                       #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:01:21 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:01:21 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

#include "util.h"

void
	vector_create(t_vector *vector, size_t elem_size, size_t capacity)
{
	vector->view.data = rt_malloc(elem_size * capacity);
	vector->view.size = 0;
	vector->view.elem_size = elem_size;
	vector->capacity = elem_size * capacity;
}

void
	vector_clear(t_vector *vector)
{
	vector->view.size = 0;
}

void
	vector_move(t_vector *vector, t_vector *other)
{
	*vector = *other;
	other->view.data = NULL;
	other->view.size = 0;
	other->capacity = 0;
}

void
	vector_destroy(t_vector *vector, t_function destroy)
{
	view_each(vector->view, destroy);
	rt_free(vector->view.data);
}

void
	vector_push(t_vector *vector, const void *element)
{
	size_t	index;
	t_view	*view;

	view = &vector->view;
	index = view_size(*view);
	view->size += view->elem_size;
	view->data = rt_reallog(view->data, &vector->capacity, view->size);
	rt_memcpy(view_get(*view, index), element, view->elem_size);
}
