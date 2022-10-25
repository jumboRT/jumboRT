/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   work.h                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:53 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:53 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORK_H
# define WORK_H

# include "state.h"
# include "mt.h"
# include "queue.h"
# include "work_compute.h"
# include "typedefs.h"

# define RT_BACKEND_SINGLE 1
# define RT_BACKEND_THREAD 2
# define RT_BACKEND_OPENCL 4
# define RT_BACKEND_SERVER 8

struct s_worker {
	t_work		*work;
	t_queue		queue;
	t_thread	thread;
	void		*ctx;
	int			backend;
};

struct s_work {
	t_worker		**workers;
	size_t			count;
	t_state			*state;
	t_uint64		work_index;
	t_uint64		work_size;
	t_uint64		work_progress;
	t_mutex			mtx;
	t_cond			cnd;
	int				pause;
	int				stop;
	size_t			paused;
	size_t			stopped;
	t_result_block	*data;
	size_t			capacity;
	t_options		*opts;
	t_uint64		*pending;
	size_t			pending_size;
	size_t			pending_capacity;
	union u_client	*client;
	int				update_stop;
	int				update_flag;
	t_thread		update_thread;
	t_cond			update_cnd;
	t_cond			progress_cnd;
	t_mutex			state_mtx;
	t_mutex			update_mtx;
};

struct s_result_block {
	t_result	*results;
	t_uint64	begin;
	t_uint64	end;
};

void	work_create(t_work *work, t_state *state, t_options *opts,
			union u_client *client);
void	work_destroy(t_work *work);
void	work_resume(t_work *work);
void	work_pause(t_work *work);
void	work_update(t_work *work);
void	work_reset(t_work *work);

void	work_send(t_work *work, t_uint64 begin, t_uint64 end);
void	work_add(t_work *work, t_start start, void *ctx, int backend);
int		work_sync(t_work *work, t_uint64 *begin, t_uint64 *end, size_t size);
void	work_done(t_work *work, t_result *results, t_uint64 begin,
			t_uint64 end);

void	work_update_start(t_work *work);
void	work_update_stop(t_work *work);
void	work_send_results(t_worker *worker, t_result *results, t_uint64 begin,
			t_uint64 end);

void	work_int_create(t_work *work);
void	work_int_destroy(t_work *work);
void	work_int_resume(t_work *work);
void	work_int_create_single(t_work *work);
void	work_int_destroy_single(t_work *work);
void	work_int_resume_single(t_work *work);
void	work_int_create_thread(t_work *work);
void	work_int_destroy_thread(t_work *work);
void	work_int_resume_thread(t_work *work);
void	work_int_create_opencl(t_work *work);
void	work_int_destroy_opencl(t_work *work);
void	work_int_resume_opencl(t_work *work);
void	work_int_create_server(t_work *work);
void	work_int_destroy_server(t_work *work);
void	work_int_resume_server(t_work *work);

void	ctx_init(t_context *ctx);
void	ctx_destroy(t_context *ctx);

void	rt_work_lock(t_work *work);
void	rt_work_unlock(t_work *work);

#endif
