#include "z.h"
#include "vector.h"

t_vector
	*lz77_deflate_int(const unsigned char *src, size_t src_size, size_t *dst_size)
{

	t_ztable			map;
	size_t				offset;
	size_t				read;
	const unsigned char	*win;
	uint32_t			next;

	ztable_create(&map, ZTABLE_SIZE);
	*dst_size = 0;
	offset = 0;

	win = src;
	while (offset < src_size)
	{
		read = min(ZHASH_LEN, src_size - offset)
		src = z_read_next(src, read, &next);
		offset += read;
	}
}

