#ifndef MT_H
# define MT_H

# ifdef RT_MT
#  include <pthread.h>

typedef pthread_t		t_thread;
typedef pthread_mutex_t	t_mutex;
typedef pthread_cond_t	t_cond;

# else

typedef int				t_thread;
typedef int				t_mutex;
typedef int				t_cond;

# endif

typedef void			*(*t_start)(void *arg);

void	thread_create(t_thread *thr, t_start start, void *arg);
void	thread_join(t_thread *thr);

void	mutex_init(t_mutex *mtx);
void	mutex_destroy(t_mutex *mtx);
void	mutex_lock(t_mutex *mtx);
void	mutex_unlock(t_mutex *mtx);

void	cond_init(t_cond *cnd);
void	cond_destroy(t_cond *cnd);
void	cond_wait(t_cond *cnd, t_mutex *mtx);
void	cond_broadcast(t_cond *cnd);

#endif
