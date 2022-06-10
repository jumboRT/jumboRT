#ifndef QUEUE_H
# define QUEUE_H

# include "mt.h"
# include "rtmath.h"

# include <stddef.h>

typedef struct s_queue	t_queue;

struct s_queue {
	void	*data;
	size_t	size;
	size_t	capacity;
	t_mutex	mtx;
};

void	queue_create(t_queue *queue);
void	queue_destroy(t_queue *queue);
void	queue_send(t_queue *queue, const void *data, size_t size);
size_t	queue_recv(t_queue *queue, void **data, size_t *capacity);

#endif
