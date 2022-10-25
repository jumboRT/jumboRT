/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vector.h                                       #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:53 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:53 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H
# define VECTOR_H

# include <stddef.h>

typedef struct s_view	t_view;
typedef struct s_vector	t_vector;
typedef void			(*t_function)(void *element);
typedef int				(*t_compare)(const void *lhs, const void *rhs);

struct s_view {
	void	*data;
	size_t	size;
	size_t	elem_size;
};

struct s_vector {
	t_view	view;
	size_t	capacity;
};

void	vector_create(t_vector *vector, size_t elem_size, size_t capacity);
void	vector_clear(t_vector *vector);
void	vector_move(t_vector *vector, t_vector *other);
void	vector_destroy(t_vector *vector, t_function destroy);
void	vector_push(t_vector *vector, const void *element);

t_view	view_view(t_view view, size_t index, size_t size);
size_t	view_size(t_view view);
void	*view_get(t_view view, size_t index);
void	view_each(t_view view, t_function func);
void	view_swap(t_view view, size_t a, size_t b);
void	view_sort(t_view view, t_compare cmp);
void	view_sort_array(void *data, size_t size, size_t elem_size,
			t_compare cmp);

#endif
