#ifndef VECTOR_H
# define VECTOR_H

# include "cl.h"

# include <stddef.h>

typedef void			(*t_destroy)(void *ptr);
typedef int				(*t_compare)(const void *a, const void *b, void *ctx);
typedef struct s_vector	t_vector;
typedef struct s_range	t_range;

struct s_vector {
	void	*data;
	size_t	size;
	size_t	capacity;
	size_t	elem_size;
};

struct s_range {
	size_t	min;
	size_t	max;
};

void	vector_init(t_vector *vec, size_t elem_size);
void	vector_destroy(t_vector *vec, t_destroy destroy);
void	vector_push_back(t_vector *vec, void *ptr);
size_t	vector_size(t_vector *vec);
void	*vector_at(t_vector *vec, size_t index);
void	*vector_front(t_vector *vec);
void	*vector_back(t_vector *vec);
void	vector_sort(t_vector *vec, t_compare compare, void *ctx);

#endif
