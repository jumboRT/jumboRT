/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   send.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:12 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:12 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

#if RT_BONUS
# ifdef RT_WINDOWS
#  include <winsock2.h>
#  include <winsock.h>
# else
#  include <sys/socket.h>
#  include <poll.h>
# endif

# include <ft_printf.h>
# include <errno.h>
# include <string.h>

int
	rt_send(int sockfd, const void *data, t_uint64 size, char **error)
{
	ssize_t	nwritten;

	nwritten = 0;
	while (size > 0)
	{
		nwritten = send(sockfd, ((const char *) data) + nwritten, size, 0);
		if (nwritten < 0)
		{
			if (errno == EINTR)
				continue ;
			if (error != NULL)
				ft_asprintf(error, "failed to send %u bytes of data: %s",
					(t_uint32) size, strerror(errno));
			return (-1);
		}
		size -= nwritten;
	}
	return (0);
}

static int
	rt_send_packet_int(int sockfd, const struct s_packet *packet, char **error)
{
	int	rc;

	rc = rt_send(sockfd, &packet->size, sizeof(packet->size), error);
	if (rc >= 0)
		rc = rt_send(sockfd, &packet->type, sizeof(packet->type), error);
	if (rc >= 0)
		rc = rt_send(sockfd, packet->data, packet->size, error);
	if (rc < 0)
		return (-1);
	return (0);
}

int
	rt_send_packet(struct s_client_base *client, const struct s_packet *packet,
			char **error)
{
	int	rc;

	mutex_lock(&client->mtx);
	rc = rt_send_packet_int(client->sockfd, packet, error);
	mutex_unlock(&client->mtx);
	return (rc);
}
#endif
