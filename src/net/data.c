/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   data.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:11 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:11 by csteenvo                                 */
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
# include <stdlib.h>
# include <stdio.h>

int
	rt_has_data(int sockfd, int timeout, char **error)
{
	struct pollfd	p;
	int				rc;

	p.fd = sockfd;
	p.events = POLLIN;
	while (1)
	{
		rc = rt_poll(&p, 1, timeout);
		if (rc < 0 && errno == EINTR)
			continue ;
		break ;
	}
	if (rc >= 0)
		return (rc);
	if (error != NULL)
		ft_asprintf(error, strerror(errno));
	return (-1);
}

int
	rt_peek(int sockfd, char **error)
{
	return (rt_has_data(sockfd, 1, error));
}
#endif
