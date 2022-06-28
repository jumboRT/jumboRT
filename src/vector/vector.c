#include "vector.h"

#include "util.h"

void
	vector_init(t_vector *vec, size_t elem_size)
{
	vec->data = NULL;
	vec->size = 0;
	vec->capacity = 0;
	vec->elem_size = elem_size;
}

void
	vector_view(t_vector *vec, t_vector *parent, size_t begin, size_t size)
{
	vec->data = (char *) parent->data + begin * parent->elem_size;
	vec->size = size * parent->elem_size;
	vec->capacity = 0;
	vec->elem_size = parent->elem_size;
}

void
	vector_destroy(t_vector *vec, t_destroy destroy)
{
	size_t	i;

	if (destroy != NULL)
	{
		i = 0;
		while (i < vector_size(vec))
		{
			destroy(vector_at(vec, i));
			i += 1;
		}
	}
	rt_free(vec->data);
}

void
	vector_push_back(t_vector *vec, const void *ptr)
{
	size_t	index;

	index = vector_size(vec);
	vec->size += vec->elem_size;
	vec->data = rt_reallog(vec->data, &vec->capacity, vec->size);
	rt_memcpy(vector_at(vec, index), ptr, vec->elem_size);
}

