#include "mt.h"

#ifndef RT_MT
# include "util.h"

void
	thread_create(t_thread *thr, t_start start, void *arg)
{
	*thr = 1;
	start(arg);
}

void
	thread_join(t_thread *thr)
{
	rt_assert(*thr, "cannot join joined thread");
	*thr = 0;
}

void
	thread_exit(void *retval)
{
	(void) retval;
	rt_assert(0, "thread_exit called in single threaded program");
}

#endif
