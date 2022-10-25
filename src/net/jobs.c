/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   jobs.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:12 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:12 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

#include <ft_printf.h>
#include <unistd.h>

#if RT_BONUS

static ssize_t
	rt_send_job(union u_client *client, t_work *work, char **error)
{
	struct s_send_work	data;
	struct s_packet		packet;
	unsigned char		buf[32];
	unsigned char		*end;

	if (work_sync(work, &data.begin, &data.end, RT_NET_JOBSIZE) == 0)
		return (0);
	end = rt_packsw(buf, data);
	rt_packet_create(&packet, end - buf, RT_SEND_WORK_PACKET, buf);
	if (rt_send_packet(&client->any, &packet, error) < 0)
		return (-1);
	return (data.end - data.begin);
}

int
	rt_send_jobs(union u_client *client, char **error)
{
	ssize_t	rc;

	while (client->viewer.active_work
		< (t_uint64) client->viewer.worker->work->opts->net_jobs
		* RT_NET_JOBSIZE)
	{
		mutex_unlock(&client->viewer.job_mtx);
		rc = rt_send_job(client,
				client->viewer.worker->work, error);
		mutex_lock(&client->viewer.job_mtx);
		if (rc <= 0)
		{
			rt_client_set_status(client, SQUIT);
			return (rc);
		}
		client->viewer.active_work += rc;
	}
	return (1);
}

void
	*rt_send_jobs_start(void *data)
{
	union u_client		*client;
	struct s_net_viewer	*viewer;
	int					rc;
	char				*error;

	client = data;
	viewer = &client->viewer;
	mutex_lock(&viewer->job_mtx);
	while (1)
	{
		rc = rt_send_jobs(client, &error);
		if (rc < 0)
			ft_fprintf(STDOUT_FILENO, "An unrecoverable error has occurred while \
synching with the server. Safely terminate this program as soon as \
is possible.\n%s\n", error);
		if (rc <= 0)
			break ;
		cond_wait(&viewer->job_cnd, &viewer->job_mtx);
	}
	mutex_unlock(&viewer->job_mtx);
	return (NULL);
}

#endif
