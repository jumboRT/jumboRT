#include "net.h"

#include "mt.h"
#include "util.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ft_printf.h>

#if RT_BONUS

# define PROTOVER 2

/* ODOT: network ping */

static void
	rt_send_results_task(void *ctx, size_t id)
{
	struct s_send_results_ctx	*info;
	struct s_packet				packet;
	struct s_send_results		data;
	uint64_t					size;
	void						*buf;

	(void) id;
	info = ctx;
	data.seq_id = info->client->any.seq_id;
	data.begin = info->begin;
	data.end = info->end;
	rt_results_deflate(&data,
		info->client->worker.work->state->world->batch_size, info->results,
		info->client->worker.opts.compression);
	size = rt_sizesr(data);
	buf = rt_malloc(size);
	rt_packsr(buf, data);
	rt_packet_create(&packet, size, RT_SEND_RESULTS_PACKET, buf);
	if (rt_send_packet(&info->client->any, &packet, NULL) < 0)
	{
		ft_fprintf(STDERR_FILENO, "probably lost connection to host\n");
		rt_client_set_status(info->client, SIDLE);
	}
	rt_free4(data.zdata, buf, info->results, info);
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
	rt_closesocket(client->any.sockfd);
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
