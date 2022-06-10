#include "work.h"

#if defined RT_WORK_SINGLE

void
	work_int_create(t_work *work)
{
	(void) work;
}

void
	work_int_destroy(t_work *work)
{
	(void) work;
}

void
	work_int_resume(t_work *work)
{
	t_result	result;
	t_context	*ctx;

	while (work->work_index < work->work_size)
	{
		result = work_compute(work->state->world, ctx, work->work_index);
		work_done(work, &result, 1);
		work->work_index += 1;
	}
}

#endif

