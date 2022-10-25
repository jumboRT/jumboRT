/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   client_loop.c                                  #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:12 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 13:06:18 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

#if RT_BONUS
# include "util.h"
# include <ft_printf.h>
# include <unistd.h>
# include <stdlib.h>

static int
	rt_recv_handle_packet(union u_client *client, char **error)
{
	struct s_packet	packet;
	int				rc;

	rc = rt_has_data(client->any.sockfd, 100, error);
	if (rc <= 0)
		return (rc);
	if (rt_recv_packet(client->any.sockfd, &packet, error) < 0)
		return (-1);
	rc = rt_handle_packet(client, packet, error);
	rt_packet_destroy(&packet);
	return (rc);
}

static void
	client_loop(union u_client *client, int (*proc)(union u_client*, char**))
{
	int				rc;
	char			*error;

	mutex_lock(&client->any.mtx);
	while (client->any.status == SRUNNING)
	{
		mutex_unlock(&client->any.mtx);
		rc = proc(client, &error);
		mutex_lock(&client->any.mtx);
		if (rc < 0)
			client->any.status = SDIED;
	}
	if (client->any.status == SDIED)
	{
		ft_fprintf(STDERR_FILENO, "An unrecoverable error has occurred while synching \
with the server. Safely terminate this program as soon as \
is possble.\n%s\n", error);
		rt_free(error);
	}
	mutex_unlock(&client->any.mtx);
}

static void
	rt_viewer_start(union u_client *client)
{
	thread_create(&client->viewer.job_thrd, rt_send_jobs_start, client);
	client_loop(client, rt_recv_handle_packet);
}

void
	rt_client_start(union u_client *client)
{
	client->any.status = SRUNNING;
	if (client->any.client_type == RT_VIEWER)
	{
		rt_viewer_start(client);
	}
	else
	{
		client_loop(client, rt_recv_handle_packet);
	}
}
#endif
