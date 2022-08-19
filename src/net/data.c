#include "net.h"

#include <ft_printf.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <poll.h>


#include <stdio.h>

int
	rt_send(int sockfd, const void *data, uint64_t size, char **error)
{
	ssize_t	nwritten;

	nwritten = 0;
	while (size > 0)
	{
		nwritten = send(sockfd, ((const char *) data) + nwritten, size, MSG_NOSIGNAL);
		if (nwritten < 0)
		{
			if (errno == EINTR)
				continue;
			if (error != NULL)
				ft_asprintf(error, "failed to send %u bytes of data: %s", (uint32_t) size, strerror(errno));
			return (-1);
		}
		size -= nwritten;
	}
	return (0);
}

ssize_t
	rt_recv(int sockfd, void *buffer, uint64_t length, char **error)
{
	ssize_t	total_read;
	ssize_t	nread;

	total_read = 0;
	while (length > 0)
	{
		nread = recv(sockfd, ((char *) buffer) + total_read, length, MSG_NOSIGNAL);
		if (nread < 0)
		{
			if (errno == EINTR)
				continue;
			if (error != NULL)
				ft_asprintf(error, "failed to receive %u bytes of data %s", (int) length, strerror(errno));
			return (-1);
		}
		length -= nread;
		total_read += nread;
	}
	return total_read;
}

int
	rt_peek(int sockfd, char **error)
{
	struct pollfd	p;
	int				rc;

	p.fd = sockfd;
	p.events = POLLIN;
	while (1)
	{
		rc = poll(&p, 1, 1);
		if (rc < 0 && errno == EINTR)
			continue;
		break;
	}
	if (rc >= 0)
		return (rc);
	if (error != NULL)
		ft_asprintf(error, strerror(errno));
	return (-1);
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
	rt_send_packet(struct s_client *client, const struct s_packet *packet,
			char **error)
{
	int	rc;

	mutex_lock(&client->mtx);
	rc = rt_send_packet_int(client->sockfd, packet, error);
	mutex_unlock(&client->mtx);
	return (rc);
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
	packet->data = malloc(packet->size);
	if (packet->data == NULL)
	{
		if (error != NULL)
			*error = NULL; /* we cannot print use asprintf in this case probably since it allocates */
		return (-1);
	}
	read = rt_recv(sockfd, packet->data, packet->size, 0);
	if (read < 0)
		return (-1);
	if ((uint64_t) read != packet->size)
	{
		free(packet->data);
		ft_asprintf(error, "could not read entire %u packet, expected %u bytes\
 got %u", (unsigned) packet->type, packet->size, (unsigned) read);
		return (-1);
	}
	return (0);
}
