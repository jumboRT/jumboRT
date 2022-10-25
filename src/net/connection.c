/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   connection.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:11 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:11 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

#if RT_BONUS
# include <sys/types.h>
# if defined RT_WINDOWS
#  include <winsock.h>
#  include <ws2def.h>
#  include <winsock2.h>
#  include <ws2tcpip.h>
# else
#  include <sys/socket.h>
#  include <netdb.h>
# endif
# include <ft_printf.h>
# include <unistd.h>
# include <string.h>
# include <libft.h>

static struct addrinfo
	*rt_get_addrinfo(const char *ip, const char *port, char **error)
{
	struct addrinfo	hints;
	struct addrinfo	*result;
	int				rc;

	ft_memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	rc = getaddrinfo(ip, port, &hints, &result);
	if (rc < 0)
	{
		if (error != NULL)
			ft_asprintf(error, "failed to get information about address: %s",
				gai_strerror(rc));
		return (NULL);
	}
	return (result);
}

static int
	rt_try_connect(struct addrinfo *info)
{
	int	sockfd;
	int	rc;

	sockfd = -1;
	while (info != NULL)
	{
		sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
		if (sockfd >= 0)
		{
			rc = connect(sockfd, info->ai_addr, info->ai_addrlen);
			if (rc == 0)
				break ;
			rt_closesocket(sockfd);
		}
		info = info->ai_next;
	}
	return (sockfd);
}

int
	rt_connect(const char *ip, const char *port, char **error)
{
	struct addrinfo	*result;
	int				sockfd;

	result = rt_get_addrinfo(ip, port, error);
	if (result == NULL)
		return (-1);
	sockfd = rt_try_connect(result);
	freeaddrinfo(result);
	if (sockfd < 0)
	{
		if (error != NULL)
			ft_asprintf(error, "failed to connect");
		return (-1);
	}
	return (sockfd);
}

# if defined RT_WINDOWS

int
	rt_closesocket(int sockfd)
{
	return (closesocket(sockfd));
}
# else

int
	rt_closesocket(int sockfd)
{
	return (close(sockfd));
}
# endif
#endif
