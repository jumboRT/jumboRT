#include "main.h"
#include "work.h"

#include <stdlib.h>
#include <unistd.h>

#if RT_BONUS

void
	rt_usleep(unsigned long us)
{
	usleep(us);
}

#else

void
	rt_usleep(unsigned long us)
{
	(void) us;
}

#endif

int
	rt_exit(t_work *work)
{
	work_pause(work);
	main_update_stop(work);
	work_destroy(work);
	exit(EXIT_SUCCESS);
	return (0);
}

void
	rt_work_lock(t_work *work)
{
	work_pause(work);
	mutex_lock(&work->state_mtx);
	while (work->work_progress < work->work_index)
		cond_wait(&work->progress_cnd, &work->state_mtx);
	work_reset(work);
	mutex_unlock(&work->state_mtx);
}

void
	rt_work_unlock(t_work *work)
{
	work_resume(work);
}
