#include "net.h"

#include <ft_printf.h>

#if RT_BONUS

# define PROTOVER 2

static int
	rt_handshake_send(union u_client *client, char **error)
{
	unsigned char		buf[17];
	struct s_handshake	data;
	struct s_packet		packet;

	data.client_type = client->any.client_type;
	data.protover = PROTOVER;
	if (client->any.client_type == RT_WORKER)
	{
		data.req_jobs = client->worker.opts.req_jobs;
		rt_packhs(buf, data, RT_WORKER);
		rt_packet_create(&packet, 17, RT_HANDSHAKE_PACKET, buf);
	}
	else
	{
		rt_packhs(buf, data, RT_VIEWER);
		rt_packet_create(&packet, 9, RT_HANDSHAKE_PACKET, buf);
	}
	if (rt_send_packet(&client->any, &packet, NULL) < 0)
	{
		if (error != NULL)
			ft_asprintf(error, "could not connect");
		return (-1);
	}
	return (0);
}

int
	rt_handshake(union u_client *client, char **error)
{
	struct s_packet		packet;

	if (rt_handshake_send(client, error) < 0)
		return (-1);
	if (rt_recv_packet(client->any.sockfd, &packet, error) < 0)
		return (-1);
	if (packet.type != RT_HANDSHAKE_PACKET)
	{
		rt_packet_destroy(&packet);
		ft_asprintf(error, "handshake failed");
		return (-1);
	}
	client->any.seq_id = -1;
	rt_packet_destroy(&packet);
	return (0);
}
#endif
