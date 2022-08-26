#include "net.h"

#include "mt.h"
#include "util.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ft_printf.h>

int
	rt_recv_handle_packet(union u_client *client, char **error)
{
	struct	s_packet	packet;
	int					rc;

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
		fprintf(stderr, "An unrecoverable error has occurred while synching \
with the server. Safely terminate this program as soon as \
is possble.\n%s\n", error);
	mutex_unlock(&client->any.mtx);
}

static int
	rt_handshake(struct s_client_base *client, char **error)
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
	rt_client_connect(struct s_client_base *client, const char *ip, const char *port,
					char **error)
{
	client->sockfd = rt_connect(ip, port, error);
	if (client->sockfd < 0)
		return (-1);
	return (rt_handshake(client, error));
}

static int
	rt_client_create(struct s_client_base *client,
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

static void
	rt_send_results_task(void *ctx, size_t id)
{
	struct s_send_results_ctx	*info;
	struct s_packet				packet;
	struct s_send_results		data;
	uint64_t					size;
	void						*buf;
	int							rc;
	(void) id;

	info = ctx;
	data.seq_id = info->client->any.seq_id;
	data.zdata = rt_results_deflate(info->results,
			info->end - info->begin, &data.zsize);
	data.begin = info->begin;
	data.end = info->end;
	size = rt_sizesr(data);
	buf = rt_malloc(size);
	rt_packsr(buf, data);
	rt_packet_create(&packet, size, RT_SEND_RESULTS_PACKET, buf);
	rc = rt_send_packet(&info->client->any, &packet, NULL);
	if (rc < 0)
	{
		fprintf(stderr, "probably lost connection to host\n"); /*TODO reconnect*/
		rt_client_set_status(info->client, SIDLE);
	}
	rt_free(data.zdata);
	rt_free(buf);
	rt_free(info->results);
	rt_free(info);
	info = ctx;
}

void
	rt_send_results(union u_client *client, t_result *results, uint64_t begin,
			uint64_t end)
{
	struct s_send_results_ctx	*ctx;
	t_task						*task;

	rt_assert(client->any.client_type == RT_WORKER,
			"tried to send results as viewer");
	task = rt_malloc(sizeof(*task));
	ctx = rt_malloc(sizeof(*ctx));
	ctx->client = client;
	ctx->results = results;
	ctx->begin = begin;
	ctx->end = end;
	task_init(task, rt_send_results_task, ctx);
	pool_fork(&client->worker.pool, task);
	pool_detach(&client->worker.pool, task);
}

int
	rt_worker_create(union u_client *client, t_options opts,
					const char *ip, const char *port)
{
	client->any.client_type = RT_WORKER;
	client->worker.work = NULL;
	client->worker.opts = opts;
	pool_create(&client->worker.pool, RT_NET_POOL_SIZE);
	return (rt_client_create(&client->any, ip, port));
}

int
	rt_viewer_create(union u_client *client,
					const char *ip, const char *port)
{
	client->any.client_type = RT_VIEWER;
	client->viewer.active_work = 0;
	client->viewer.worker = NULL;
	/* TODO: these are not destroyed on error */
	mutex_init(&client->viewer.job_mtx);
	cond_init(&client->viewer.job_cnd);
	return (rt_client_create(&client->any, ip, port));
}

static void
	rt_viewer_start(union u_client *client)
{
	struct s_cjob_request	request;
	struct s_packet			packet;
	unsigned char			*data;
	unsigned char			*end;
	char 					*error;

	rt_string_create(&request.scene,
			client->viewer.worker->work->opts->scene_file);
	request.width = client->viewer.worker->work->opts->width;
	request.height = client->viewer.worker->work->opts->height;
	data = rt_malloc(sizeof(request.width) * 3 + request.scene.len);
	end = rt_packcjr(data, request);
	rt_packet_create(&packet, end - data, RT_JOB_REQUEST_PACKET, data);
	if (rt_send_packet(&client->any, &packet, &error) < 0)
	{
		fprintf(stderr, "failed to start viewer: %s\n", error);
		rt_free(error);
		return;
	}
	rt_string_destroy(&request.scene);
	rt_packet_destroy(&packet);
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

void
	rt_client_set_status(union u_client *client, enum e_status status)
{
	mutex_lock(&client->any.mtx);
	client->any.status = status;
	mutex_unlock(&client->any.mtx);
}

void
	rt_client_destroy(union u_client *client)
{
	rt_client_set_status(client, SQUIT);
	if (client->any.client_type == RT_VIEWER)
		thread_join(&client->viewer.job_thrd);
	else
		pool_destroy(&client->worker.pool);
#if defined RT_WINDOWS
	closesocket(client->any.sockfd);
#else
	close(client->any.sockfd);
#endif
	/* This is not ok, we cannot destroy the mutex before it is actually done */
	mutex_destroy(&client->any.mtx);
}
