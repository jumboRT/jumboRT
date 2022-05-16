#include "mt.h"

#ifdef RT_MT

void
	cond_init(t_cond *cnd)
{
	int	ret;

	ret = pthread_cond_init(cnd, NULL);
	rt_assert(!ret, "cond init failed");
}

void
	cond_destroy(t_cond *cnd)
{
	int	ret;

	ret = pthread_cond_destroy(cnd);
	rt_assert(!ret, "cond destroy failed");
}

int
	cond_wait(t_cond *cnd, t_mutex *mtx)
{
	int	ret;

	ret = pthread_cond_wait(cnd, mtx);
	rt_assert(!ret, "cond wait failed");
	return (0);
}

void
	cond_broadcast(t_cond *cnd)
{
	int	ret;

	ret = pthread_cond_broadcast(cnd);
	rt_assert(!ret, "cond wait failed");
}

#endif
