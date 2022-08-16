#include "net.h"

int
	rt_send_job(t_work *work, char **error)
{
	uint64_t	begin;
	uint64_t	end;

}

int
	rt_send_jobs(struct s_client *client, char **error)
{
	while (client->active_jobs < RT_NET_MAX_JOBS)
	{
		if (rt_send_job(client->work, error) < 0)
			return (-1);
		client->active_jobs += 1;
	}
	return (0);
}
