#include "net.h"

#include <ft_printf.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>

int
	rt_send(int sockfd, const void *data, uint64_t size, char **error)
{
	ssize_t	nwritten;

	nwritten = 0;
	while (1)
	{
		nwritten = send(sockfd, ((const char *) data) + nwritten, size, MSG_NOSIGNAL);
		if (nwritten < 0)
		{
			if (errno == EINTR)
				continue;
			if (error != NULL)
				ft_asprintf(error, "failed to send %llu bytes of data %s\n", size, strerror(errno));
			return (-1);
		}
		else if ((uint64_t) nwritten < size)
		{
			size -= nwritten;
		}
		else
		{
			break;
		}
	}
	return (0);
}

ssize_t
	rt_recv(int sockfd, void *buffer, uint64_t length, char **error)
{
	ssize_t	total_read;
	ssize_t	nread;

	total_read = 0;
	while (1)
	{
		nread = recv(sockfd, ((char *) buffer) + total_read, length, MSG_NOSIGNAL);
		if (nread < 0)
		{
			if (errno == EINTR)
				continue;
			if (error != NULL)
				ft_asprintf(error, "failed to receive %llu bytes of data %s\n", length, strerror(errno));
			return (-1);
		}
		else if ((uint64_t) nread < length)
		{
			length -= nread;
			total_read += nread;
		}
		else
		{
			break;
		}
	}
	return total_read;
}

int
	rt_send_packet(int sockfd, const struct s_packet *packet, char **error)
{
	int	rc;

	rc = 0;
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
	rt_recv_packet(int sockfd, struct s_packet *packet, char **error)
{
	int	rc;

	rc = rt_recv(sockfd, &packet->size, sizeof(packet->size), error);
	if (rc < 0)
		return (-1);
	rc = rt_recv(sockfd, &packet->type, sizeof(packet->type), error);
	if (rc < 0)
		return (-1);
	packet->data = malloc(packet->size);
	if (packet->data == NULL)
	{
		if (error != NULL)
			*error = NULL; /* we cannot print use asprintf in this case probable since it allocates */
		return (-1);
	}
	rc = rt_recv(sockfd, packet->data, packet->size, 0);
	if (rc < 0)
		return (-1);
	return (0);
}
