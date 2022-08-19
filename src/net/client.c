#include "net.h"

#include "mt.h"
#include "util.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ft_printf.h>

int
	rt_recv_packets(struct s_client *client, char **error)
{
	struct s_packet	packet;
	int				rc;

	while (1)
	{
		rc = rt_recv_packet(client->sockfd, &packet, error);
		if (rc < 0)
			return (-1);
		if (rt_handle_packet(client, packet, error) < 0)
		{
			rt_packet_destroy(&packet);
			return (-1);
		}
		rc = rt_peek(client->sockfd, error);
		if (rc <= 0)
		{
			rt_packet_destroy(&packet);
			return (rc);
		}
		rt_packet_destroy(&packet);
	}
	return (0);
}

static void
	client_loop(struct s_client *client, int (*proc)(struct s_client*, char**))
{
	int				rc;
	char			*error;

	mutex_lock(&client->mtx);
	while (client->status == SRUNNING)
	{
		mutex_unlock(&client->mtx);
		rc = proc(client, &error);
		mutex_lock(&client->mtx);
		if (rc < 0)
			client->status = SDIED;
	}
	if (client->status == SDIED)
		fprintf(stderr, "An unrecoverable error has occurred while synching \
with the server. Safely terminate this program as soon as \
is possble.\n%s\n", error);
	mutex_unlock(&client->mtx);
}

static int
	rt_handshake(struct s_client *client, char **error)
{
	struct s_packet packet;

	rt_packet_create(&packet, sizeof(uint8_t),
				RT_HANDSHAKE_PACKET, &client->client_type);
	if (rt_send_packet(client, &packet, error) < 0)
	{
		if (error != NULL)
		{
			rt_free(*error);
			ft_asprintf(error, "could not connect");
		}
		return (-1);
	}
	if (rt_recv_packet(client->sockfd, &packet, error) < 0)
		return (-1);
	if (packet.type != RT_HANDSHAKE_PACKET)
	{
		rt_packet_destroy(&packet);
		ft_asprintf(error, "handshake failed");
		return (-1);
	}
	if (client->client_type == RT_VIEWER)
	{
		if (packet.size != 8)
		{
			ft_asprintf(error, "handshake packet has wrong size: %u",
					(int) packet.size);
			rt_packet_destroy(&packet);
			return (-1);
		}
		rt_upacku64(packet.data, &client->seq_id);
	}
	rt_packet_destroy(&packet);
	return (0);
}

static int 
	rt_client_connect(struct s_client *client, const char *ip, const char *port,
					char **error)
{
	client->sockfd = rt_connect(ip, port, error);
	if (client->sockfd < 0)
		return (-1);
	return (rt_handshake(client, error));
}

static int
	rt_client_create(struct s_client *client,
					const char *ip, const char *port)
{
	char	*error;

	mutex_init(&client->mtx);
	if (rt_client_connect(client, ip, port, &error) < 0)
	{
		fprintf(stderr, "%s:%s: %s\n", ip, port, error);
		mutex_destroy(&client->mtx);
		rt_free(error);
		return (-1);
	}
	client->status = SIDLE;
	return (0);
}

void
	rt_send_results(struct s_client *client, t_result *results, uint64_t count)
{
	struct s_packet			packet;
	struct s_send_results	data;
	uint64_t				size;
	void					*buf;
	int						rc;

	data.seq_id = client->seq_id;
	data.count = count;
	data.results = results;
	size = rt_sizesr(data);
	buf = rt_malloc(size);
	rt_packsr(buf, data);
	rt_packet_create(&packet, size, RT_SEND_RESULTS_PACKET, buf);
	rc = rt_send_packet(client, &packet, NULL);
	if (rc < 0)
	{
		fprintf(stderr, "probably lost connection to host\n"); /*TODO reconnect*/
		rt_client_set_status(client, SIDLE);
	}
	rt_free(buf);
}
	

int
	rt_worker_create(struct s_client *client, t_options opts,
					const char *ip, const char *port)
{
	client->client_type = RT_WORKER;
	client->impl.worker.work = NULL;
	client->impl.worker.client = client;
	client->impl.worker.opts = opts;
	return (rt_client_create(client, ip, port));
}

int
	rt_viewer_create(struct s_client *client,
					const char *ip, const char *port)
{
	client->client_type = RT_VIEWER;
	client->impl.viewer.active_work = 0;
	client->impl.viewer.client = client;
	client->impl.viewer.worker = NULL;
	/* TODO: these are not destroyed on error */
	mutex_init(&client->impl.viewer.job_mtx);
	cond_init(&client->impl.viewer.job_cnd);
	return (rt_client_create(client, ip, port));
}

static void
	rt_viewer_start(struct s_net_viewer *viewer)
{
	struct s_cjob_request	request;
	struct s_packet			packet;
	unsigned char			*data;
	unsigned char			*end;
	char 					*error;

	rt_string_create(&request.scene, viewer->worker->work->opts->scene_file);
	request.width = viewer->worker->work->opts->width;
	request.height = viewer->worker->work->opts->height;
	data = rt_malloc(sizeof(request.width) * 3 + request.scene.len);
	end = rt_packcjr(data, request);
	rt_packet_create(&packet, end - data, RT_JOB_REQUEST_PACKET, data);
	if (rt_send_packet(viewer->client, &packet, &error) < 0)
	{
		fprintf(stderr, "failed to start viewer: %s\n", error);
		rt_free(error);
		return;
	}
	rt_string_destroy(&request.scene);
	rt_packet_destroy(&packet);
	thread_create(&viewer->handler_thread, rt_send_jobs_start, viewer->client);
	client_loop(viewer->client, rt_recv_packets);
}

void
	rt_client_start(struct s_client *client)
{

	client->status = SRUNNING;
	if (client->client_type == RT_VIEWER)
	{
		rt_viewer_start(&client->impl.viewer);
	}
	else
	{
		client_loop(client, rt_recv_packets);
	}
}

void
	rt_client_set_status(struct s_client *client, enum e_status status)
{
	mutex_lock(&client->mtx);
	client->status = status;
	mutex_unlock(&client->mtx);
}

void
	rt_client_destroy(struct s_client *client)
{
	rt_client_set_status(client, SQUIT);
	if (client->client_type == RT_VIEWER)
		thread_join(&client->impl.viewer.handler_thread);
	/* This is not ok, we cannot destroy the mutex before it is actually done */
	mutex_destroy(&client->mtx);
}
