#include "work.h"

#include "util.h"
#include "net.h"

static void
	*work_start(void *data)
{
	t_worker		*worker;
	struct s_client	*client;

	worker = data;
	client = worker->ctx;
	client->impl.viewer.worker = worker;
	rt_client_start(worker->ctx);
	return (NULL);
}

void
	work_int_create_server(t_work *work)
{
	struct s_client	*client;

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

void
	work_int_resume_server(t_work *work)
{
	(void) work;
}

