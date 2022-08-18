#include "net.h"

#include <stdio.h>

static int
	rt_send_job(int sockfd, t_work *work, char **error)
{
	struct s_send_work	data;
	struct s_packet		packet;
	unsigned char		buf[32];
	unsigned char		*end;

	if (work_sync(work, &data.begin, &data.end, RT_NET_JOBSIZE) == 0)
		return (0);
	end = rt_packsw(buf, data);
	rt_packet_create(&packet, end - buf, RT_SEND_WORK_PACKET, buf);
	if (rt_send_packet(sockfd, &packet, error) < 0)
		return (-1);
	return (1);
}

int
	rt_send_jobs(struct s_client *client, char **error)
{
	int	rc;

	while (client->impl.viewer.active_work < RT_NET_MAX_JOBS * RT_NET_JOBSIZE)
	{
		rc = rt_send_job(client->sockfd,
				client->impl.viewer.worker->work, error);
		if (rc < 0)
			return (-1);
		else if (rc == 0)
		{
			rt_client_set_status(client, SQUIT);
			return (0);
		}
		client->impl.viewer.active_work += RT_NET_JOBSIZE;
	}
	return (0);
}
