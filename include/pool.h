/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   pool.h                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:51 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:51 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef POOL_H
# define POOL_H

# define RT_POOL_MAX_TASKS 1024

# include "typedefs.h"
# include "mt.h"
# include "queue.h"

# include <stddef.h>

typedef void				(*t_pool_start)(void *ctx, size_t id);

struct s_pool {
	t_thread	*threads;
	size_t		count;
	void		*data;
	size_t		size;
	size_t		capacity;
	t_mutex		mtx;
	t_cond		cnd;
	int			stop;
};

struct s_task {
	t_pool_start	start;
	void			*ctx;
	size_t			count;
	size_t			done;
	int				detached;
};

struct s_task_item {
	t_task	*task;
	size_t	id;
};

void	*pool_create_int(void *ctx);

void	task_init(t_task *task, t_pool_start start, void *ctx);
void	pool_create(t_pool *pool, size_t count);
void	pool_destroy(t_pool *pool);
void	pool_fork(t_pool *pool, t_task *task);
void	pool_join(t_pool *pool, t_task *task);
void	pool_detach(t_pool *pool, t_task *task);

#endif
