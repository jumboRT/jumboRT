#include "queue.h"

#include "util.h"

void
	queue_create(t_queue *queue)
{
	queue->data = NULL;
	queue->size = 0;
	queue->capacity = 0;
	mutex_init(&queue->mtx);
	cond_init(&queue->cnd);
}

void
	queue_destroy(t_queue *queue)
{
	mutex_destroy(&queue->mtx);
	cond_destroy(&queue->cnd);
	rt_free(queue->data);
}

void
	queue_send(t_queue *queue, const void *data, size_t size)
{
	size_t	old_size;
	size_t	new_size;

	mutex_lock(&queue->mtx);
	while (queue->size + size > RT_QUEUE_MAX)
		cond_wait(&queue->cnd, &queue->mtx);
	old_size = queue->size;
	new_size = old_size + size;
	queue->data = rt_reallog(queue->data, &queue->capacity, new_size);
	rt_memcpy((char *) queue->data + old_size, data, size);
	queue->size = new_size;
	mutex_unlock(&queue->mtx);
}

size_t
	queue_recv(t_queue *queue, void **data, size_t *capacity)
{
	size_t	size;

	mutex_lock(&queue->mtx);
	size = queue->size;
	*data = rt_reallog(*data, capacity, size);
	rt_memcpy(*data, queue->data, size);
	queue->size = 0;
	mutex_unlock(&queue->mtx);
	cond_broadcast(&queue->cnd);
	return (size);
}
