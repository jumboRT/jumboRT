#include "z.h"

#include "util.h"

void
	zring_create(t_zring *ring, size_t n)
{
	ring->data = rt_malloc(sizeof(*ring->data) * n);
	ring->size = n;
	ring->index = 0;
	ring->int_size = 0;
}

void
	zring_destroy(t_zring *ring)
{
	rt_free(ring->data);
	ring->size = 0;
	ring->index = 0;
	ring->int_size = 0;
}

t_zchain
	*zring_at(t_zring *ring, int16_t index)
{
	if (index == ZEMPTY)
		return (NULL);
	return (&ring->data[index]);
}

int
	zring_isfull(t_zring *ring)
{
	return (ring->int_size == ring->size);
}

void
	zring_advance(t_zring *ring)
{
	ring->index = (ring->index + 1) % ring->size;
	if (ring->int_size != ring->size)
		ring->int_size++;
}
