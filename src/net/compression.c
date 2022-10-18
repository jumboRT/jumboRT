#if defined RT_BONUS
#include "net.h"

#include "z.h"
#include "ser.h"
#include "util.h"

void
	rt_results_deflate(struct s_send_results *packet, size_t batch_size, t_result *results, int level)
{
	unsigned char	*data;
	unsigned char	*ptr;
	size_t			index;
	t_vec			color;
	uint64_t		samples;

	data = rt_malloc(6 * ((packet->end - packet->begin + batch_size - 2) / batch_size + 1));
	ptr = data;
	index = packet->begin;
	while (index < packet->end)
	{
		color = vec_0();
		samples = 0;
		while (index < packet->end)
		{
			color = vec_add(color, results[index - packet->begin].color);
			samples += 1;
			index += 1;
			if (index % batch_size == 0)
				break ;
		}
		color = vec_scale(color, 1.0 / samples);
		ptr = rt_packhvec(ptr, color);
	}
	packet->zdata = z_deflate(data, ptr - data, &packet->zsize, level);
	// printf("deflated size: %zu\n", packet->zsize, batch_size);
	rt_free(data);
}

t_result
	*rt_results_inflate(struct s_send_results packet, size_t batch_size)
{
	unsigned char	*data;
	unsigned char	*ptr;
	size_t			index;
	size_t			size;
	t_vec			color;
	t_result		*result;

	data = z_inflate(packet.zdata, packet.zsize, &size);
	result = rt_malloc((packet.end - packet.begin) * sizeof(*result));
	ptr = data;
	index = packet.begin;
	while (index < packet.end)
	{
		rt_assert((size_t) (ptr - data + 6) <= size, "corrupted packet");
		ptr = rt_upackhvec(ptr, &color);
		while (index < packet.end)
		{
			result[index - packet.begin].color = color;
			index += 1;
			if (index % batch_size == 0)
				break ;
		}
	}
	rt_free(data);
	return (result);
}
#endif
