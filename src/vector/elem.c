#include "vector.h"

size_t
	vector_size(t_vector *vec)
{
	return (vec->size / vec->elem_size);
}

void
	*vector_at(t_vector *vec, size_t index)
{
	return ((char *) vec->data + vec->elem_size * index);
}

void
	*vector_front(t_vector *vec)
{
	size_t	size;

	size = vector_size(vec);
	if (size == 0)
		return (NULL);
	return (vector_at(vec, 0));
}

void
	*vector_back(t_vector *vec)
{
	size_t	size;

	size = vector_size(vec);
	if (size == 0)
		return (NULL);
	return (vector_at(vec, size - 1));
}
