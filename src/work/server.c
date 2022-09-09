#include "work.h"

#include "util.h"
#include "net.h"

static void
	*work_start(void *data)
{
	t_worker		*worker;
	union u_client	*client;

	worker = data;
	client = worker->ctx;
	client->viewer.worker = worker;
	rt_client_start(worker->ctx);
	return (NULL);
}

void
	work_int_create_server(t_work *work)
{
	union u_client	*client;

	client = rt_malloc(sizeof(*client));
	rt_viewer_create(client, work->opts->net_ip, work->opts->net_port);
	work_add(work, work_start, client, RT_BACKEND_SERVER);
}

void
	work_int_destroy_server(t_work *work)
{
	size_t	i;

	i = 0;
	while (i < work->count)
	{
		if (work->workers[i]->backend == RT_BACKEND_SERVER)
		{
			rt_client_destroy(work->workers[i]->ctx);
			rt_free(work->workers[i]->ctx);
		}
		i += 1;
	}
}

static struct s_packet 
	work_int_create_request_packet(t_work *work)
{
	struct s_packet			packet;
	struct s_cjob_request	request;

	request.width = work->opts->width;
	request.height = work->opts->height;
	request.cam_pos = work->state->world->camera.org;
	request.cam_dir = work->state->world->camera.dir;
	request.cam_fov = work->state->world->camera.fov;
	request.cam_focus = work->state->world->camera.focus;
	request.cam_blur = work->state->world->camera.blur;
	rt_string_create(&request.scene_file, work->opts->scene_file);
	rt_string_create(&request.scene_key, work->opts->key);
	request.render_mode = work->state->world->render_mode;
	request.batch_size = work->state->world->batch_size;
	request.trace_batch_size = work->state->world->trace_batch_size;
	packet.size = rt_sizecjr(request);
	packet.type = RT_JOB_REQUEST_PACKET;
	packet.data = rt_malloc(packet.size);
	rt_packcjr(packet.data, request);
	rt_string_destroy(&request.scene_file);
	rt_string_destroy(&request.scene_key);
	return (packet);
}

void
	work_int_resume_server(t_work *work)
{
	struct s_packet	packet;
	union u_client	*client;
	size_t			i;
	char			*error;

	i = 0;
	packet = work_int_create_request_packet(work);
	while (i < work->count)
	{
		if (work->workers[i]->backend == RT_BACKEND_SERVER)
		{
			client = work->workers[i]->ctx;
			if (rt_send_packet((struct s_client_base *) client, &packet, &error) < 0)
				rt_free(error);
		}
		i += 1;
	}
	rt_packet_destroy(&packet);
}

