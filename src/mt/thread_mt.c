#include "mt.h"

#ifdef RT_MT

void
	thread_create(t_thread *thr, t_start start, void *arg)
{
	int	ret;

	ret = pthread_create(thr, NULL, start, arg);
	rt_assert(!ret, "thread create failed");
}

void
	thread_join(t_thread *thr)
{
	int	ret;

	ret = pthread_join(*thr, NULL);
	rt_assert(!ret, "thread join failed");
}

#endif
