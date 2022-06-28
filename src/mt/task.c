#include "pool.h"

void
	task_create(t_task *task)
{
	mutex_init(&task->mtx);
	cond_init(&task->cnd);
	task->done = 0;
}

void
	task_destroy(t_task *task)
{
	mutex_destroy(&task->mtx);
	cond_destroy(&task->cnd);
}

void
	task_wait(t_task *task)
{
	mutex_lock(&task->mtx);
	while (!task->done)
		cond_wait(&task->cnd, &task->mtx);
	mutex_unlock(&task->mtx);
}

void
	task_done(t_task *task)
{
	mutex_lock(&task->mtx);
	task->done = 1;
	cond_broadcast(&task->cnd);
	mutex_unlock(&task->mtx);
}

