/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   handler.c                                      #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:12 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:12 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

#include <unistd.h>
#include <limits.h>
#include <ft_printf.h>

#if RT_BONUS

static int
	rt_handle_ping(union u_client *client, struct s_packet packet,
					char **error)
{
	struct s_packet	response;

	(void) packet;
	rt_packet_create(&response, 0, RT_PONG_PACKET, NULL);
	return (rt_send_packet(&client->any, &response, error));
}

static int
	rt_handle_send_work(union u_client *client, struct s_packet packet,
					char **error)
{
	struct s_send_work	data;

	if (client->any.client_type == RT_VIEWER)
	{
		ft_asprintf(error, "received send-work packet as a viewer");
		return (-1);
	}
	rt_upacksw(packet.data, &data);
	work_send(client->worker.work, data.begin, data.end);
	return (0);
}

static int
	rt_handle_log(union u_client *client, struct s_packet packet,
					char **error)
{
	(void) error;
	(void) client;
	if (packet.size > INT_MAX)
	{
		ft_fprintf(STDOUT_FILENO, "warning: received log packet over %d bytes long, \
				message will be truncated\n", INT_MAX);
	}
	ft_printf("log: %.*s\n", (int)((unsigned int) packet.size % 1 << 31),
		(char *) packet.data);
	return (0);
}

int
	rt_handle_packet(union u_client *client, struct s_packet packet,
					char **error)
{
	if (packet.type == RT_PING_PACKET)
		return (rt_handle_ping(client, packet, error));
	if (packet.type == RT_PONG_PACKET)
		return (0);
	if (packet.type == RT_JOB_REQUEST_PACKET)
		return (rt_handle_job_request(client, packet, error));
	if (packet.type == RT_SEND_WORK_PACKET)
		return (rt_handle_send_work(client, packet, error));
	if (packet.type == RT_SEND_RESULTS_PACKET)
		return (rt_handle_send_results(client, packet, error));
	if (packet.type == RT_LOG_PACKET)
		return (rt_handle_log(client, packet, error));
	if (error != NULL)
		ft_asprintf(error, "unknown packet type %d\n", packet.type);
	return (-1);
}
#endif
