/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   net.h                                          #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:50 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:50 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef NET_H
# define NET_H

# include "mt.h"
# include "work.h"
# include "pool.h"
# include <stdint.h>
# include <sys/types.h> /* TODO make this portable */

# define RT_NET_JOBSIZE			262144ULL
# define RT_NET_POOL_SIZE		8

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
	t_uint64	size;
	t_uint8		type;
	void		*data;
};

struct s_client_base {
	t_mutex			mtx;
	t_uint8			client_type;
	int				sockfd;
	t_uint64		seq_id;
	enum e_status	status;
	t_pool			pool;
};

struct s_net_viewer {
	struct s_client_base	base;
	t_worker				*worker;
	t_uint64				active_work;
	t_mutex					job_mtx;
	t_cond					job_cnd;
	t_thread				job_thrd;
};

struct s_net_worker {
	struct s_client_base	base;
	t_work					*work;
	t_options				opts;
};

union u_client {
	struct s_client_base	any;
	struct s_net_viewer		viewer;
	struct s_net_worker		worker;
};

struct s_string {
	t_uint64	len;
	char		*str;
};

struct s_handshake {
	t_uint8		client_type;
	t_uint64	protover;
	t_uint64	req_jobs;
};

struct s_send_work {
	t_uint64	begin;
	t_uint64	end;
};

struct s_cjob_request {
	t_uint64		width;
	t_uint64		height;
	t_vec			cam_pos;
	t_vec			cam_dir;
	float			cam_fov;
	float			cam_focus;
	float			cam_blur;
	struct s_string	scene_file;
	struct s_string	scene_key;
	t_uint64		render_mode;
	t_uint64		batch_size;
	t_uint64		trace_batch_size;
};

struct s_sjob_request {
	t_uint64		seq_id;
	t_uint64		width;
	t_uint64		height;
	t_vec			cam_pos;
	t_vec			cam_dir;
	float			cam_fov;
	float			cam_focus;
	float			cam_blur;
	struct s_string	scene_file;
	struct s_string	scene_key;
	t_uint64		render_mode;
	t_uint64		batch_size;
	t_uint64		trace_batch_size;
};

struct s_send_results {
	t_uint64	seq_id;
	t_uint64	begin;
	t_uint64	end;
	t_uint64	zsize;
	void		*zdata;
};

struct s_send_results_ctx {
	union u_client	*client;
	t_result		*results;
	t_uint64		begin;
	t_uint64		end;
};

struct s_handle_send_results_ctx {
	union u_client	*client;
	struct s_packet	packet;
};

int			rt_connect(const char *ip, const char *port, char **error);
int			rt_closesocket(int sockfd);
int			rt_send(int sockfd, const void *data, t_uint64 size, char **error);
ssize_t		rt_recv(int sockfd, void *buffer, t_uint64 length, char **error);
int			rt_peek(int sockfd, char **error);
int			rt_has_data(int sockfd, int timeout, char **error);

int			rt_send_packet(struct s_client_base *client,
				const struct s_packet *packet, char **error);
int			rt_recv_packet(int sockfd, struct s_packet *packet, char **error);

void		*rt_packstr(void *dst, struct s_string str);
void		*rt_packhs(void *dst, struct s_handshake packet, int type);
void		*rt_packsw(void *dst, struct s_send_work packet);
void		*rt_packcjr(void *dst, struct s_cjob_request packet);
void		*rt_packsr(void *dst, struct s_send_results packet);
void		*rt_upackstr(void *src, struct s_string *str);
void		*rt_upacksr(void *src, struct s_send_results *dst);
void		*rt_upacksjr(void *src, struct s_sjob_request *dst);
void		*rt_upacksw(void *src, struct s_send_work *dst);

void		rt_results_deflate(struct s_send_results *packet, size_t batch_size,
				t_result *results, int level);
t_result	*rt_results_inflate(struct s_send_results packet,
				size_t batch_size);
t_uint64	rt_sizesr(struct s_send_results packet);
t_uint64	rt_sizecjr(struct s_cjob_request packet);

void		rt_packet_create(struct s_packet *packet, t_uint64 data_size,
				t_uint8 type, void *data);
void		rt_packet_destroy(struct s_packet *packet);

int			rt_handle_packet(union u_client *client, struct s_packet packet,
				char **error);

int			rt_handshake(union u_client *client, char **error);

int			rt_worker_create(union u_client *client, t_options opts,
				const char *ip, const char *port);
int			rt_viewer_create(union u_client *client,
				const char *ip, const char *port);
void		rt_client_start(union u_client *client);
void		rt_client_set_status(union u_client *client, enum e_status status);
void		rt_client_destroy(union u_client *client);

void		*rt_send_jobs_start(void *data);
int			rt_send_jobs(union u_client *client, char **error);
void		rt_send_results(union u_client *client, t_result *results,
				t_uint64 begin, t_uint64 end);

void		rt_string_create(struct s_string *dst, const char *str);
void		rt_string_destroy(struct s_string *string);

void		rt_create_new_work(struct s_net_worker *worker,
				struct s_sjob_request request);
int			rt_handle_send_results(union u_client *client,
				struct s_packet packet, char **error);
int			rt_handle_job_request(union u_client *client,
				struct s_packet packet, char **error);
#endif
