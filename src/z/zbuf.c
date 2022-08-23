#include "z.h"

#include "util.h"

void
	zbuf_create(t_zbuf *zb, void *data, size_t size)
{
	zb->data = data;
	zb->index = 0;
	zb->size = size;
	zb->capacity = size;
	zb->bit = 0;
}

void
	zbuf_next(t_zbuf *zb)
{
	if (zb->bit != 0)
	{
		zb->bit = 0;
		zb->index += 1;
	}
}

unsigned int
	zbuf_read(t_zbuf *zb, int count)
{
	unsigned int	data;
	int				i;
	int				n;

	int tmp = count;

	data = 0;
	i = 0;
	while (count > 0)
	{
		rt_assert(zb->index < zb->size, "zbuf_read: buffer overflow");
		n = 8 - zb->bit;
		if (count < n)
			n = count;
		data |= ((zb->data[zb->index] >> zb->bit) & ((1U << n) - 1)) << i;
		i += n;
		count -= n;
		zb->bit += n;
		zb->index += zb->bit == 8;
		zb->bit %= 8;
	}
	printf("%d: %d\n", tmp, data);
	return (data);
}

void
	zbuf_write(t_zbuf *zb, int count, unsigned int data)
{
	int	n;

	while (count > 0)
	{
		if (zb->bit == 0)
		{
			zb->data = rt_reallog(zb->data, &zb->capacity, zb->size + 1);
			zb->data[zb->size] = 0;
			zb->size += 1;
		}
		n = 8 - zb->bit;
		if (count < n)
			n = count;
		zb->data[zb->size - 1] |= (data & ((1U << n) - 1)) << zb->bit;
		data >>= n;
		count -= n;
		zb->bit += n;
		zb->bit %= 8;
	}
}

void
	zbuf_copy(t_zbuf *zb, t_zbuf *src, size_t size)
{
	rt_assert(zb->bit == 0, "zbuf_copy: zb->bit != 0");
	rt_assert(src->bit == 0, "zbuf_copy: src->bit != 0");
	rt_assert(src->index + size <= src->size, "zbuf_copy: buffer overflow");
	zb->data = rt_reallog(zb->data, &zb->capacity, zb->size + size);
	rt_memcpy(zb->data + zb->size, src->data + src->index, size);
	zb->size += size;
	src->index += size;
}

void
	zbuf_repeat(t_zbuf *zb, size_t dist, size_t size)
{
	size_t	i;

	rt_assert(zb->bit == 0, "zbuf_repeat: zb->bit != 0");
	rt_assert(dist <= zb->size, "zbuf_repeat: buffer underflow");
	zb->data = rt_reallog(zb->data, &zb->capacity, zb->size + size);
	i = 0;
	while (i < size)
	{
		zb->data[zb->size + i] = zb->data[zb->size - dist + i];
		i += 1;
	}
	zb->size += size;
}

