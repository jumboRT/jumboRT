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
	rt_handle_ping(struct s_client *client, struct s_packet packet,
					char **error)
{
	struct s_packet	response;

	(void) packet;
	rt_packet_create(&response, 0, RT_PONG_PACKET, NULL);
	return (rt_send_packet(client->sockfd, &response, error));
}

static int
	rt_unimplemented(struct s_packet packet, char **error)
{
	if (error != NULL)
		ft_asprintf(error, "handling of packet type %d is not implemented",
			packet.type);
	fprintf(stderr, "yikes\n");
	return (-1);
}

static void 
	rt_create_new_work(struct s_net_worker *worker)
{
	t_state	*state;

	if (worker->work != NULL)
	{
		work_pause(worker->work);
		work_update_stop(worker->work);
		rt_free(worker->work->state->world);
		rt_free(worker->work->state);
		/* TODO: become thanos */
		work_destroy(worker->work);
		rt_free(worker->work);
	}
	worker->work = rt_malloc(sizeof(*worker->work));
	state = rt_malloc(sizeof(*state));
	state->image = NULL;
	state->world = rt_malloc(sizeof(*state->world));
	world_create(state->world);
	state->world->img_meta.width = worker->opts.width;
	state->world->img_meta.height = worker->opts.height;
	world_load(state->world, worker->opts.scene_file);
	world_accel(state->world);
	work_create(worker->work, state, &worker->opts, worker->client);
	work_update_start(worker->work);
	work_resume(worker->work);
}

static int
	rt_handle_job_request(struct s_client *client, struct s_packet packet,
					char **error)
{
	struct s_sjob_request	request;

	if (client->client_type == RT_VIEWER)
	{
		ft_asprintf(error, "received a job-request packet as a viewer");
		return (-1);
	}
	rt_upacksjr(packet.data, &request);
	client->impl.worker.opts.width = request.width;
	client->impl.worker.opts.height = request.height;
	client->impl.worker.opts.scene_file = ft_strdup(request.scene.str);
	client->seq_id = request.seq_id;
	rt_create_new_work(&client->impl.worker);
	rt_string_destroy(&request.scene);
	return (0);
}

static int
	rt_handle_send_work(struct s_client *client, struct s_packet packet,
					char **error)
{
	struct s_send_work data;

	if (client->client_type == RT_VIEWER)
	{
		ft_asprintf(error, "received send-work packet as a viewer");
		return (-1);
	}
	rt_upacksw(packet.data, &data);
	work_send(client->impl.worker.work, data.begin, data.end);
	return (0);
}

static int
	rt_handle_send_results(struct s_client *client, struct s_packet packet,
					char **error)
{
	struct s_send_results	data;

	if (client->client_type == RT_WORKER)
	{
		ft_asprintf(error, "received send-results as a worker");
		return (-1);
	}
	rt_upacksr(packet.data, &data);
	if (data.seq_id == client->seq_id)
	{
		work_send_results(client->impl.viewer.worker, data.results, data.count);
		client->impl.viewer.active_work -= data.count;
	}
	return (0);
}

static int
	rt_handle_log(struct s_client *client, struct s_packet packet,
					char **error)
{
	(void) error; /* TODO handle this correctly using the new string struct */
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
	rt_handle_packet(struct s_client *client, struct s_packet packet,
					char **error)
{
	fprintf(stderr, "got me a %d packet\n", packet.type);
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
	ft_asprintf(error, "unknown packet type %d\n", packet.type);
	return (-1);
}
