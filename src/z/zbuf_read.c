#include "z.h"

#include "util.h"

unsigned int
	zbuf_read(t_zbuf *zb, int count)
{
	unsigned int	data;
	int				i;

	data = zbuf_peek(zb, count, &i);
	zbuf_skip(zb, count);
	return (data);
}

void
	zbuf_skip(t_zbuf *zb, int count)
{
	rt_assert((zb->size - zb->index) * 8 - zb->bit >= (unsigned) count,
		"zbuf_skip: buffer overflow");
	zb->bit += count;
	zb->index += zb->bit / 8;
	zb->bit %= 8;
}

static unsigned int
	zbuf_peek_fast(t_zbuf *zb, int *i)
{
	unsigned int	data;

	data = 0;
	if (zb->index + 0 < zb->size)
	{
		data |= (unsigned int) zb->data[zb->index + 0] << 0 >> zb->bit;
		*i += 8;
	}
	if (zb->index + 1 < zb->size)
	{
		data |= (unsigned int) zb->data[zb->index + 1] << 8 >> zb->bit;
		*i += 8;
	}
	if (zb->index + 2 < zb->size)
	{
		data |= (unsigned int) zb->data[zb->index + 2] << 16 >> zb->bit;
		*i += 8;
	}
	return (data);
}

unsigned int
	zbuf_peek(t_zbuf *zb, int count, int *i)
{
	unsigned int	data;

	rt_assert(count <= 16, "zbuf_peek: can peek at most 16 bits");
	if (zb->index + 3 < zb->size)
	{
		data = *(uint32_t *)(zb->data + zb->index) >> zb->bit;
		*i = count;
	}
	else
	{
		*i = 0;
		data = zbuf_peek_fast(zb, i);
		if (count < *i)
			*i = count;
	}
	return (data & ((1 << count) - 1));
}
