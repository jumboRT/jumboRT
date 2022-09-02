#include "mt.h"
#include "util.h"

#ifdef RT_MT

void
	thread_create(t_thread *thr, t_start start, void *arg)
{
	int				ret;
	pthread_attr_t	attr;

	ret = pthread_attr_init(&attr);
	rt_assert(!ret, "attr init failed");
	ret = pthread_attr_setstacksize(&attr, 16777216);
	rt_assert(!ret, "attr setstacksize failed");
	ret = pthread_create(thr, &attr, start, arg);
	rt_assert(!ret, "thread create failed");
}

void
	thread_join(t_thread *thr)
{
	int	ret;

	ret = pthread_join(*thr, NULL);
	rt_assert(!ret, "thread join failed");
}

void
	thread_exit(void *retval)
{
	pthread_exit(retval);
}

#endif
