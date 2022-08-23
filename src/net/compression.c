#include "net.h"

#include "z.h"
#include "util.h"

/* TODO: make sure all memory access happens within the buffer */
void
	*rt_results_deflate(t_result *results, size_t count, size_t *zsize)
{
	unsigned char	*data;
	unsigned char	*ptr;
	unsigned char	*tmp;
	size_t			index;
	size_t			chunk_offset;
	size_t			chunk_count;

	data = rt_malloc(28 * count);
	ptr = data;
	index = 0;
	while (index < count)
	{
		chunk_offset = results[index].index;
		chunk_count = 0;
		ptr = rt_packu64(ptr, chunk_offset);
		tmp = ptr;
		ptr = rt_packu64(ptr, chunk_count);
		while (index < count && results[index].index == chunk_offset + chunk_count)
		{
			ptr = rt_packvec(ptr, results[index].color);
			index += 1;
			chunk_count += 1;
		}
		rt_packu64(tmp, chunk_count);
	}
	tmp = z_deflate(data, ptr - data, zsize);
	printf("results packet uncompressed=%zu compressed=%zu\n", (size_t) (ptr - data), *zsize);
	rt_free(data);
	return (tmp);
}

/* TODO: make sure all memory access happens within the buffer */
t_result
	*rt_results_inflate(struct s_send_results packet, size_t *count)
{
	unsigned char	*data;
	unsigned char	*ptr;
	size_t			size;
	size_t			index;
	uint64_t		chunk_offset;
	uint64_t		chunk_count;
	t_result		*result;

	*count = 0;
	result = NULL;
	data = z_inflate(packet.zdata, packet.zsize, &size);
	ptr = data;
	while ((size_t) (ptr - data) < size)
	{
		ptr = rt_upacku64(ptr, &chunk_offset);
		ptr = rt_upacku64(ptr, &chunk_count);
		result = rt_realloc(result, (*count) * sizeof(*result), (*count + chunk_count) * sizeof(*result));
		index = 0;
		while (index < chunk_count)
		{
			result[*count + index].index = chunk_offset + index;
			ptr = rt_upackvec(ptr, &result[*count + index].color);
			index += 1;
		}
		*count += chunk_count;
	}
	rt_free(data);
	return (result);
}
