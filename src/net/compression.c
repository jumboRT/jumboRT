#include "net.h"

#include "z.h"
#include "util.h"

void
	*rt_results_deflate(t_result *results, size_t count, size_t *zsize)
{
	unsigned char	*data;
	unsigned char	*ptr;
	size_t			index;

	data = rt_malloc(6 * count);
	ptr = data;
	index = 0;
	while (index < count)
	{
		ptr = rt_packvec(ptr, results[index].color);
		index += 1;
	}
	ptr = z_deflate(data, ptr - data, zsize);
	printf("results packet uncompressed=%zu compressed=%zu\n", 6 * count, *zsize);
	rt_free(data);
	return (ptr);
}

t_result
	*rt_results_inflate(struct s_send_results packet)
{
	unsigned char	*data;
	unsigned char	*ptr;
	size_t			index;
	size_t			size;
	t_result		*result;

	data = z_inflate(packet.zdata, packet.zsize, &size);
	result = rt_malloc((packet.end - packet.begin) * sizeof(*result));
	rt_assert(size == (packet.end - packet.begin) * 6, "corrupted packet");
	ptr = data;
	index = 0;
	while (index < packet.end - packet.begin)
	{
		ptr = rt_upackvec(ptr, &result[index].color);
		index += 1;
	}
	rt_free(data);
	return (result);
}
