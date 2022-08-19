#ifndef NET_H
# define NET_H

#include "mt.h"
#include "work.h"
#include <stdint.h>
#include <sys/types.h> /* TODO make this portable */

# define RT_NET_MAX_JOBS		16
# define RT_NET_JOBSIZE			65536

# define RT_HANDSHAKE_PACKET	0x00
# define RT_PING_PACKET			0x01
# define RT_PONG_PACKET			0x02
# define RT_JOB_REQUEST_PACKET	0x03
# define RT_SEND_WORK_PACKET	0x04
# define RT_SEND_RESULTS_PACKET	0x05
# define RT_LOG_PACKET			0xFF

# define RT_WORKER				0x00
# define RT_VIEWER				0x01

enum e_status {
	SIDLE,
	SRUNNING,
	SQUIT,
	SDIED
};

struct s_packet {
	uint64_t	size;
	uint8_t		type;
	void		*data;
};

struct s_net_viewer {
	struct s_client	*client;
	t_worker		*worker;
	uint64_t		active_work;
	t_mutex			job_mtx;
	t_cond			job_cnd;
	t_thread		handler_thread;/*TODO rename to job_thrd or smth like that*/
};

struct s_net_worker {
	struct s_client	*client;
	t_work			*work;
	t_options		opts;
};

struct s_client {
	t_mutex				mtx;
	uint8_t				client_type;
	int					sockfd;
	uint64_t			seq_id;
	enum e_status		status;
	union {
		struct s_net_viewer	viewer;
		struct s_net_worker	worker;
	} impl;
};

struct s_string {
	uint64_t	len;
	char		*str;
};

struct s_handshake {
	uint8_t	client_type;
};

struct s_send_work {
	uint64_t	begin;
	uint64_t	end;
};

struct s_cjob_request {
	uint64_t		width;
	uint64_t		height;
	struct s_string	scene;
};

struct s_sjob_request {
	uint64_t		seq_id;
	uint64_t		width;
	uint64_t		height;
	struct s_string	scene;
};

struct s_send_results {
	uint64_t	seq_id;
	uint64_t	count;
	t_result	*results;
};

int		rt_connect(const char *ip, const char *port, char **error);
int		rt_send(int sockfd, const void *data, uint64_t size, char **error);
ssize_t	rt_recv(int sockfd, void *buffer, uint64_t length, char **error);
int		rt_peek(int sockfd, char **error);

int		rt_send_packet(struct s_client *client, const struct s_packet *packet, char **error);
int		rt_recv_packet(int sockfd, struct s_packet *packet, char **error);

void	*rt_packstr(void *dst, struct s_string str);
void	*rt_packu64(void *dst, uint64_t i);
void	*rt_packfl(void *dst, FLOAT f);
void	*rt_packvec(void *dst, t_vec vec);
void	*rt_packhs(void *dst, struct s_handshake packet);
void	*rt_packsw(void *dst, struct s_send_work packet);
void	*rt_packcjr(void *dst, struct s_cjob_request packet);
void	*rt_packsr(void *dst, struct s_send_results packet);
void	*rt_upackstr(void *src, struct s_string *str);
void	*rt_upacku64(void *src, uint64_t *i);
void	*rt_upacksr(void *src, struct s_send_results *dst);
void	*rt_upacksjr(void *src, struct s_sjob_request *dst);
void	*rt_upacksw(void *src, struct s_send_work *dst);

uint64_t	rt_sizesr(struct s_send_results packet);

void	rt_packet_create(struct s_packet *packet, uint64_t data_size,
					uint8_t type, void *data);
void	rt_packet_destroy(struct s_packet *packet);

int		rt_handle_packet(struct s_client *client, struct s_packet packet,
					char **error);

int		rt_worker_create(struct s_client *client, t_options opts,
						const char *ip, const char *port);
int		rt_viewer_create(struct s_client *client,
						const char *ip, const char *port);
void	rt_client_start(struct s_client *client);
void	rt_client_set_status(struct s_client *client, enum e_status status);
void	rt_client_destroy(struct s_client *client);

void	*rt_send_jobs_start(void *data);
int		rt_send_jobs(struct s_client *client, char **error);
void	rt_send_results(struct s_client *client, t_result *results,
						uint64_t count);
int		rt_recv_packets(struct s_client *client, char **error);

void	rt_string_create(struct s_string *dst, const char *str);
void	rt_string_destroy(struct s_string *string);
#endif
