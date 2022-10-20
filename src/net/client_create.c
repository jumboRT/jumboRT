#include "net.h"

#include <stdlib.h>
#include <unistd.h>
#include <ft_printf.h>

#if RT_BONUS

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
#endif
