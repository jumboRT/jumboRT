#if RT_BONUS
# include "net.h"

# include "util.h"
# include <ft_printf.h>

static void
	rt_handle_send_results_task(void *ctx, size_t id)
{
	struct s_send_results				data;
	t_result							*results;
	struct s_handle_send_results_ctx	*info;

	(void) id;
	info = ctx;
	rt_upacksr(info->packet.data, &data);
	if (data.seq_id == info->client->any.seq_id)
	{
		results = rt_results_inflate(data,
				info->client->viewer.worker->work->state->world->batch_size);
		work_send_results(info->client->viewer.worker, results, data.begin,
			data.end);
		mutex_lock(&info->client->viewer.job_mtx);
		info->client->viewer.active_work -= data.end - data.begin;
		cond_broadcast(&info->client->viewer.job_cnd);
		mutex_unlock(&info->client->viewer.job_mtx);
		rt_free(results);
	}
	rt_free(data.zdata);
	rt_free(info->packet.data);
	rt_free(info);
}

int
	rt_handle_send_results(union u_client *client, struct s_packet packet,
					char **error)
{
	struct s_handle_send_results_ctx	*ctx;
	t_task								*task;

	if (client->any.client_type == RT_WORKER)
	{
		ft_asprintf(error, "received send-results as a worker");
		return (-1);
	}
	task = rt_malloc(sizeof(*task));
	ctx = rt_malloc(sizeof(*ctx));
	ctx->client = client;
	ctx->packet = packet;
	ctx->packet.data = rt_memdup(ctx->packet.data, ctx->packet.size);
	task_init(task, rt_handle_send_results_task, ctx);
	pool_fork(&client->any.pool, task);
	pool_detach(&client->any.pool, task);
	return (0);
}

#endif
