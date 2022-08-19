#include "z.h"

void
	*z_deflate(void *src, size_t src_size, size_t *dst_size)
{
	t_zbuf	ib;
	t_zbuf	ob;
	size_t	index;

	zbuf_create(&ib, src, src_size);
	zbuf_create(&ob, NULL, 0);
	index = 0;
	while (index + 65535 < src_size)
	{
		zbuf_write(&ob, 1, 0);
		zbuf_write(&ob, 2, 0);
		ob.bit = 0;
		zbuf_write(&ob, 16, 65535);
		zbuf_write(&ob, 16, 0);
		zbuf_copy(&ob, &ib, 65535);
		index += 65535;
	}
	zbuf_write(&ob, 1, 1);
	zbuf_write(&ob, 2, 0);
	ob.bit = 0;
	zbuf_write(&ob, 16, src_size - index);
	zbuf_write(&ob, 16, ~(src_size - index));
	zbuf_copy(&ob, &ib, src_size - index);
	*dst_size = ob.size;
	return (ob.data);
}
