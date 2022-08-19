#include "net.h"

#include "z.h"
#include "util.h"

void
	*rt_results_deflate(t_result *results, size_t count, size_t *zsize)
{
	size_t	idx;
	size_t	psize;
	void	*pcolors;
	void	*tmp;

	psize = sizeof(FLOAT) * 3 * count;
	pcolors = rt_malloc(psize); 
	tmp = pcolors;
	idx = 0;
	while (idx < count)
	{
		tmp = rt_packvec(tmp, results[idx].color);
		++idx;
	}
	tmp = z_deflate(pcolors, psize, zsize);
	rt_free(pcolors);
	return (tmp);
}
/*
t_vec
	*rt_colors_inflate(void *zsrc, size_t size, size_t *count)
{
	void	*pcolors;
	size_t	psize;
	size_t	idx;
	size_t	colr_count;
	t_vec	*colors;

	pcolors = z_inflate(zsrc, size, &psize);
	colr_count = psize / (sizeof(FLOAT) * 3);
	colors = rt_malloc(sizeof(t_vec) * colr_count);
	idx = 0;
	while (idx < colr_count)
	{
		pcolors = rt_upackvec(pcolors, &colors[idx]);
		++idx;
	}
	if (count != NULL)
		*count = colr_count;
	rt_free(pcolors);
	return (colors);
}
*/

t_result
	*rt_results_inflate(union u_client *client, struct s_send_results packet)
{
	void	*pcolors;
	void	*tmp;
	size_t	psize;
	size_t	idx;
	t_result *results;

	pcolors = z_inflate(packet.zdata, packet.zsize, &psize);
	tmp = pcolors;
	rt_assert(packet.count * sizeof(FLOAT) * 3 == psize, "count != zcount");
	results = rt_malloc(sizeof(t_result) * packet.count);
	idx = 0;
	while (idx < packet.count)
	{
		results[idx].index = packet.index++;
		tmp = rt_upackvec(tmp, &results[idx].color);
		++idx;
	}
	rt_free(pcolors);
	return (results);
}
