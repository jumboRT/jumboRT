#ifndef NET_H
# define NET_H

#include "mt.h"
#include "work.h"
#include <stdint.h>
#include <sys/types.h> /* TODO make this portable */

enum e_status {
	SRUNNING,
	SQUIT,
	SDIED
};

struct s_packet {
	uint64_t	size;
	uint8_t		type;
	void		*data;
};

struct s_client {
	t_thread		thread;
	t_mutex			mtx;
	t_work			*work;
	enum e_status	status;
	uint64_t		active_jobs;
};

struct s_handshake {
	uint8_t	client_type;
};

int		rt_connect(const char *ip, const char *port, char **error);
int		rt_send(int sockfd, const void *data, uint64_t size, char **error);
ssize_t	rt_recv(int sockfd, void *buffer, uint64_t length, char **error);

int		rt_send_packet(int sockfd, const struct s_packet *packet, char **error);
int		rt_recv_packet(int sockfd, struct s_packet *packet, char **error);

void	*rt_packu64(void *dest, uint64_t i);
void	*rt_packhs(void *dest, struct s_handshake packet);

void	rt_client_create(struct s_client *client, t_work *work);
void	rt_client_stop(struct s_client *client);
void	rt_client_destroy(struct s_client *client);
#endif
