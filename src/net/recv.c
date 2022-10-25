/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   recv.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:13 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:13 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

#if RT_BONUS
# include "util.h"
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

ssize_t
	rt_recv(int sockfd, void *buffer, t_uint64 length, char **error)
{
	ssize_t	total_read;
	ssize_t	nread;

	total_read = 0;
	while (length > 0)
	{
		nread = recv(sockfd, ((char *) buffer) + total_read, length, 0);
		if (nread < 0 && errno == EINTR)
			continue ;
		if (nread <= 0)
		{
			if (error != NULL && nread < 0)
				ft_asprintf(error, "failed to receive %u bytes of data %s",
					(int) length, strerror(errno));
			if (error != NULL && nread == 0)
				ft_asprintf(error, "failed to receive %u bytes of data",
					(int) length);
			return (-1);
		}
		length -= nread;
		total_read += nread;
	}
	return (total_read);
}

int
	rt_recv_packet(int sockfd, struct s_packet *packet, char **error)
{
	ssize_t	read;

	read = rt_recv(sockfd, &packet->size, sizeof(packet->size), error);
	if (read < 0)
		return (-1);
	read = rt_recv(sockfd, &packet->type, sizeof(packet->type), error);
	if (read < 0)
		return (-1);
	packet->data = rt_malloc(packet->size);
	read = rt_recv(sockfd, packet->data, packet->size, 0);
	if (read < 0)
		return (-1);
	if ((t_uint64) read != packet->size)
	{
		rt_free(packet->data);
		ft_asprintf(error, "could not read entire %u packet, expected %u bytes\
 got %u", (unsigned) packet->type, packet->size, (unsigned) read);
		return (-1);
	}
	return (0);
}
#endif
