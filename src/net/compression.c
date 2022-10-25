/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   compression.c                                  #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:11 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 13:09:57 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"
#include "z.h"
#include "ser.h"
#include "util.h"

#if RT_BONUS

void
	rt_results_deflate(struct s_send_results *packet, size_t sz,
			t_result *results, int level)
{
	unsigned char	*dp[2];
	size_t			index;
	t_vec			color;
	t_uint64		samples;

	dp[0] = rt_malloc(6 * ((packet->end - packet->begin + sz - 2) / sz + 1));
	dp[1] = dp[0];
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
			if (index % sz == 0)
				break ;
		}
		color = vec_scale(color, 1.0 / samples);
		dp[1] = rt_packhvec(dp[1], color);
	}
	packet->zdata = z_deflate(dp[0], dp[1] - dp[0], &packet->zsize, level);
	rt_free(dp[0]);
}

t_result
	*rt_results_inflate(struct s_send_results packet, size_t batch_size)
{
	unsigned char	*data;
	unsigned char	*ptr;
	uint64_t		si[2];
	t_vec			color;
	t_result		*result;

	data = z_inflate(packet.zdata, packet.zsize, &si[1]);
	result = rt_malloc((packet.end - packet.begin) * sizeof(*result));
	ptr = data;
	si[0] = packet.begin;
	while (si[0] < packet.end)
	{
		rt_assert((size_t)(ptr - data + 6) <= si[1], "corrupted packet");
		ptr = rt_upackhvec(ptr, &color);
		while (si[0] < packet.end)
		{
			result[si[0] - packet.begin].color = color;
			si[0] += 1;
			if (si[0] % batch_size == 0)
				break ;
		}
	}
	rt_free(data);
	return (result);
}
#endif
