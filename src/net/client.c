#include "net.h"

#include "mt.h"
#include "util.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ft_printf.h>

#if RT_BONUS

#define PROTOVER 2

/* ODOT: network ping */

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
	{
		ft_fprintf(STDERR_FILENO, "An unrecoverable error has occurred while synching \
with the server. Safely terminate this program as soon as \
is possble.\n%s\n", error);
		rt_free(error);
	}
	mutex_unlock(&client->any.mtx);
}

static int
	rt_handshake(union u_client *client, char **error)
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
	if (rt_send_packet(&client->any, &packet, error) < 0)
	{
		if (error != NULL)
		{
			rt_free(*error);
			ft_asprintf(error, "could not connect");
		}
		return (-1);
	}
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

static int 
	rt_client_connect(union u_client *client, const char *ip, const char *port,
					char **error)
{
	client->any.sockfd = rt_connect(ip, port, error);
	if (client->any.sockfd < 0)
		return (-1);
	return (rt_handshake(client, error));
}

static int
	rt_client_create(union u_client *client,
					const char *ip, const char *port)
{
	char	*error;

	mutex_init(&client->any.mtx);
	if (rt_client_connect(client, ip, port, &error) < 0)
	{
		ft_fprintf(STDERR_FILENO, "%s:%s: %s\n", ip, port, error);
		exit(EXIT_FAILURE);
	}
	client->any.status = SIDLE;
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
	data.begin = info->begin;
	data.end = info->end;
	rt_results_deflate(&data, info->client->worker.work->state->world->batch_size, info->results, info->client->worker.opts.compression);
	size = rt_sizesr(data);
	buf = rt_malloc(size);
	rt_packsr(buf, data);
	rt_packet_create(&packet, size, RT_SEND_RESULTS_PACKET, buf);
	rc = rt_send_packet(&info->client->any, &packet, NULL);
	if (rc < 0)
	{
		ft_fprintf(STDERR_FILENO, "probably lost connection to host\n");
		rt_client_set_status(info->client, SIDLE);
	}
	rt_free(data.zdata);
	rt_free(buf);
	rt_free(info->results);
	rt_free(info);
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
	pool_fork(&client->any.pool, task);
	pool_detach(&client->any.pool, task);
}

int
	rt_worker_create(union u_client *client, t_options opts,
					const char *ip, const char *port)
{
	client->any.client_type = RT_WORKER;
	client->worker.work = NULL;
	client->worker.opts = opts;
	pool_create(&client->any.pool, RT_NET_POOL_SIZE);
	return (rt_client_create(client, ip, port));
}

int
	rt_viewer_create(union u_client *client,
					const char *ip, const char *port)
{
	client->any.client_type = RT_VIEWER;
	client->viewer.active_work = 0;
	client->viewer.worker = NULL;
	mutex_init(&client->viewer.job_mtx);
	cond_init(&client->viewer.job_cnd);
	pool_create(&client->any.pool, RT_NET_POOL_SIZE);
	return (rt_client_create(client, ip, port));
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
	else if (client->any.client_type == RT_WORKER)
	{
		if (client->worker.work != NULL)
		{
			rt_free((void *) client->worker.opts.scene_file);
			rt_free((void *) client->worker.opts.key);
		}
	}
	pool_destroy(&client->any.pool);
#if defined RT_WINDOWS
	closesocket(client->any.sockfd);
#else
	close(client->any.sockfd);
#endif
	mutex_destroy(&client->any.mtx);
}

#else

void
	rt_send_results(union u_client *client, t_result *results, uint64_t begin,
			uint64_t end)
{
	(void) client;
	(void) results;
	(void) begin;
	(void) end;
	ft_printf("Yup, we totally just sent some results\n");
	ft_printf("Nothing funny going on here\n");
	ft_printf("Definitely do not look at the line below this one\n");
	rt_free(results);
}

#endif
