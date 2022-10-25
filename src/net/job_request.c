/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   job_request.c                                  #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:11 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:11 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"
#include "ser.h"

#if RT_BONUS

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
	t_uint64	seq_id;

	(void) error;
	rt_upacku64(packet.data, &seq_id);
	mutex_lock(&client->base.mtx);
	client->base.seq_id = seq_id;
	mutex_unlock(&client->base.mtx);
	return (0);
}

int
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

#endif
