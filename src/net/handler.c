#include "net.h"

#include "util.h"
#include "parser.h"
#include "world_impl.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <libft.h>
#include <ft_printf.h>

static int
	rt_handle_ping(union u_client *client, struct s_packet packet,
					char **error)
{
	struct s_packet	response;

	(void) packet;
	rt_packet_create(&response, 0, RT_PONG_PACKET, NULL);
	return (rt_send_packet(&client->any, &response, error));
}

static void 
	rt_create_new_work(struct s_net_worker *worker, struct s_sjob_request request)
{
	t_state	*state;

	if (worker->work != NULL 
			&& ft_strcmp(worker->opts.scene_file, request.scene_file.str) == 0
			&& ft_strcmp(worker->opts.key, request.scene_key.str) == 0
			&& (uint64_t) worker->opts.width == request.width
			&& (uint64_t) worker->opts.height == request.height)
	{
		state = worker->work->state;
		rt_work_lock(worker->work);
		state->world->render_mode = request.render_mode;
		state->world->batch_size = request.batch_size;
		camera_set(state->world, &state->world->camera, request.cam_pos,
				request.cam_dir, request.cam_fov);
		worker->work->work_size = 0;
		rt_work_unlock(worker->work);
		return;
	}
	if (worker->work != NULL)
	{
		work_pause(worker->work);
		work_update_stop(worker->work);
		world_destroy(worker->work->state->world);
		work_destroy(worker->work);
		rt_free(worker->work->state->world);
		rt_free(worker->work->state);
		rt_free(worker->work);
		rt_free((char *) worker->work->opts->scene_file);
		rt_free((char *) worker->work->opts->key);
	}
	worker->work = rt_malloc(sizeof(*worker->work));
	state = rt_malloc(sizeof(*state));
	state->image = NULL;
	state->world = rt_malloc(sizeof(*state->world));
	world_create(state->world);
	state->world->render_mode = request.render_mode;
	state->world->batch_size = request.batch_size;
	state->world->img_meta.width = request.width;
	state->world->img_meta.height = request.height;
	world_load(state->world, request.scene_file.str, request.scene_key.str);
	world_accel(state->world);
	camera_set(state->world, &state->world->camera, request.cam_pos, request.cam_dir, request.cam_fov);
	work_create(worker->work, state, &worker->opts, (union u_client *) worker);
	work_update_start(worker->work);
	worker->opts.width = request.width;
	worker->opts.height = request.height;
	worker->opts.scene_file = ft_strdup(request.scene_file.str);
	worker->opts.key = ft_strdup(request.scene_key.str);
	work_resume(worker->work);
}

static int
	rt_handle_job_request_worker(struct s_net_worker *client,
					struct s_packet packet, char **error)
{
	struct s_sjob_request	request;

	rt_upacksjr(packet.data, &request);
	rt_create_new_work(client, request);
	client->base.seq_id = request.seq_id;
	rt_string_destroy(&request.scene_file);
	rt_string_destroy(&request.scene_key);
	(void) error;
	return (0);
}

static int
	rt_handle_job_request_viewer(struct s_net_viewer *client,
					struct s_packet packet, char **error)
{
	uint64_t	seq_id;

	(void) error;
	rt_upacku64(packet.data, &seq_id);
	mutex_lock(&client->base.mtx);
	client->base.seq_id = seq_id;
	mutex_unlock(&client->base.mtx);
	return (0);
}

static int
	rt_handle_job_request(union u_client *client, struct s_packet packet,
					char **error)
{
	if (client->any.client_type == RT_VIEWER)
	{
		return (rt_handle_job_request_viewer(&client->viewer, packet, error));
	}
	else
	{
		return (rt_handle_job_request_worker(&client->worker, packet, error));
	}
	return (0);
}

static int
	rt_handle_send_work(union u_client *client, struct s_packet packet,
					char **error)
{
	struct s_send_work data;

	if (client->any.client_type == RT_VIEWER)
	{
		ft_asprintf(error, "received send-work packet as a viewer");
		return (-1);
	}
	rt_upacksw(packet.data, &data);
	work_send(client->worker.work, data.begin, data.end);
	return (0);
}

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
		results = rt_results_inflate(data, info->client->viewer.worker->work->state->world->batch_size);
		work_send_results(info->client->viewer.worker, results, data.begin, data.end);
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

static int
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

static int
	rt_handle_log(union u_client *client, struct s_packet packet,
					char **error)
{
	(void) error;
	(void) client;
	if (packet.size > INT_MAX)
	{
		fprintf(stderr, "warning: received log packet over %d bytes long, \
				message will be truncated\n", INT_MAX);
	}
	ft_printf("log: %.*s\n", (int) packet.size, (char *) packet.data);
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
