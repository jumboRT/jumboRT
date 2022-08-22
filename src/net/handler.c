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
	rt_create_new_work(union u_client *client)
{
	t_state	*state;

	if (client->worker.work != NULL)
	{
		work_pause(client->worker.work);
		work_update_stop(client->worker.work);
		rt_free(client->worker.work->state->world);
		rt_free(client->worker.work->state);
		/* TODO: become thanos */
		work_destroy(client->worker.work);
		rt_free(client->worker.work);
	}
	client->worker.work = rt_malloc(sizeof(*client->worker.work));
	state = rt_malloc(sizeof(*state));
	state->image = NULL;
	state->world = rt_malloc(sizeof(*state->world));
	world_create(state->world);
	state->world->img_meta.width = client->worker.opts.width;
	state->world->img_meta.height = client->worker.opts.height;
	world_load(state->world, client->worker.opts.scene_file, NULL);
	world_accel(state->world);
	work_create(client->worker.work, state, &client->worker.opts, client);
	work_update_start(client->worker.work);
	work_resume(client->worker.work);
}

static int
	rt_handle_job_request(union u_client *client, struct s_packet packet,
					char **error)
{
	struct s_sjob_request	request;

	if (client->any.client_type == RT_VIEWER)
	{
		ft_asprintf(error, "received a job-request packet as a viewer");
		return (-1);
	}
	rt_upacksjr(packet.data, &request);
	client->worker.opts.width = request.width;
	client->worker.opts.height = request.height;
	client->worker.opts.scene_file = ft_strdup(request.scene.str);
	client->any.seq_id = request.seq_id;
	rt_create_new_work(client);
	rt_string_destroy(&request.scene);
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

static int
	rt_handle_send_results(union u_client *client, struct s_packet packet,
					char **error)
{
	struct s_send_results	data;
	t_result				*results;
	size_t					count;

	if (client->any.client_type == RT_WORKER)
	{
		ft_asprintf(error, "received send-results as a worker");
		return (-1);
	}
	rt_upacksr(packet.data, &data);
	if (data.seq_id == client->any.seq_id)
	{
		results = rt_results_inflate(data, &count);
		work_send_results(client->viewer.worker, results, count);
		mutex_lock(&client->viewer.job_mtx);
		client->viewer.active_work -= count;
		cond_broadcast(&client->viewer.job_cnd);
		mutex_unlock(&client->viewer.job_mtx);
		rt_free(results);
	}
	rt_free(data.zdata);
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
