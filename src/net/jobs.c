#include "net.h"

#include <stdio.h>

static ssize_t 
	rt_send_job(struct s_client *client, t_work *work, char **error)
{
	struct s_send_work	data;
	struct s_packet		packet;
	unsigned char		buf[32];
	unsigned char		*end;

	if (work_sync(work, &data.begin, &data.end, RT_NET_JOBSIZE) == 0)
		return (0);
	end = rt_packsw(buf, data);
	rt_packet_create(&packet, end - buf, RT_SEND_WORK_PACKET, buf);
	if (rt_send_packet(client, &packet, error) < 0)
		return (-1);
	return (data.end - data.begin);
}

int
	rt_send_jobs(struct s_client *client, char **error)
{
	ssize_t	rc;

	while (client->impl.viewer.active_work < RT_NET_MAX_JOBS * RT_NET_JOBSIZE)
	{
		mutex_unlock(&client->impl.viewer.job_mtx);
		rc = rt_send_job(client,
				client->impl.viewer.worker->work, error);
		mutex_lock(&client->impl.viewer.job_mtx);
		if (rc <= 0)
		{
			rt_client_set_status(client, SQUIT);
			return (rc);
		}
		client->impl.viewer.active_work += rc;
	}
	return (1);
}

void
	*rt_send_jobs_start(void *data)
{
	struct s_client		*client;
	struct s_net_viewer	*viewer;
	int					rc;
	char				*error;

	client = data;
	viewer = &client->impl.viewer;
	mutex_lock(&viewer->job_mtx);
	while (1)
	{
		rc = rt_send_jobs(client, &error);
		if (rc < 0)
			fprintf(stderr, "An unrecoverable error has occurred while \
synching with the server. Safely terminate this program as soon as \
is possible.\n%s\n", error);
		if (rc <= 0)
			break ;
		cond_wait(&viewer->job_cnd, &viewer->job_mtx);
	}
	mutex_unlock(&viewer->job_mtx);
	return (NULL);
}

