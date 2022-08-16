#include "net.h"

#include "mt.h"
#include <stdio.h>

static void
	*client_loop(void *ctx)
{
	struct s_client *client;
	int				rc;
	char			*error;

	client = ctx;
	mutex_lock(&client->mtx);
	while (client->status == SRUNNING)
	{
		mutex_unlock(&client->mtx);
		rc = rt_send_jobs(client, &error);
		if (rc >= 0)
			rt_recv_jobs(client, &error);
		mutex_lock(&client->mtx);
		if (rc < 0)
			client->status = SDIED;
	}
	mutex_unlock(&client->mtx);
	if (client->status == SDIED)
		fprintf(stderr, "An unrecoverable error has occurred while synching \
				with the workers. Safely terminate this program as soon as \
				is possble.\n%s\n", error);
	return (NULL);
}

void
	rt_client_create(struct s_client *client, t_work *work)
{
	thread_create(&client->thread, client_loop, client);
	mutex_init(&client->mtx);
	client->status = SRUNNING;
	client->active_jobs = 0;
	client->work = work;
}

void
	rt_client_stop(struct s_client *client)
{
	mutex_lock(&client->mtx);
	client->status = SQUIT;
	mutex_unlock(&client->mtx);
	thread_join(&client->thread);
}

void
	rt_client_destroy(struct s_client *client)
{
	mutex_destroy(&client->mtx);
}
