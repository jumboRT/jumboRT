#include "mt.h"

#ifndef RT_MT

void
	cond_init(t_cond *cnd)
{
	(void) cnd;
}

void
	cond_destroy(t_cond *cnd)
{
	(void) cnd;
}

int
	cond_wait(t_cond *cnd, t_mutex *mtx)
{
	(void) cnd;
	(void) mtx;
	return (1);
}

void
	cond_broadcast(t_cond *cnd)
{
	(void) cnd;
}

#endif
