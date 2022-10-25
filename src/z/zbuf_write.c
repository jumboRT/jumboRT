/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   zbuf_write.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:14 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:14 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "z.h"
#include "util.h"

void
	zbuf_write_aligned(t_zbuf *zb, int count, unsigned int data)
{
	int	i;

	rt_assert(zb->bit == 0, "zbuf_write_aligned: zb->bit != 0");
	rt_assert(count % 8 == 0, "zbuf_write_aligned: count % 8 != 0");
	zb->data = rt_reallog(zb->data, &zb->capacity, zb->size + count / 8);
	i = 0;
	while (i < count)
	{
		zb->data[zb->size] = (data >> i) & 0xFF;
		zb->size += 1;
		i += 8;
	}
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
	zbuf_copy_forward(void *dst, const void *src, size_t size)
{
	unsigned char		*dst_cpy;
	const unsigned char	*src_cpy;

	dst_cpy = dst;
	src_cpy = src;
	while (size)
	{
		*dst_cpy = *src_cpy;
		dst_cpy += 1;
		src_cpy += 1;
		size -= 1;
	}
}

void
	zbuf_repeat(t_zbuf *zb, size_t dist, size_t size)
{
	rt_assert(zb->bit == 0, "zbuf_repeat: zb->bit != 0");
	rt_assert(dist <= zb->size, "zbuf_repeat: buffer underflow");
	zb->data = rt_reallog(zb->data, &zb->capacity, zb->size + size);
	zbuf_copy_forward(zb->data + zb->size, zb->data + zb->size - dist,
		sizeof(zb->data[0]) * size);
	zb->size += size;
}
