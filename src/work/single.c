#include "work.h"
#include "util.h"

void
	work_int_create_single(t_work *work)
{
	(void) work;
}

void
	work_int_destroy_single(t_work *work)
{
	(void) work;
}

void
	work_int_resume_single(t_work *work)
{
	t_result	*result;
	t_context	ctx;

	ctx_init(&ctx);
	while (work->work_index < work->work_size)
	{
		result = rt_malloc(sizeof(*result));
		*result = work_compute(work->state->world, &ctx, work->work_index);
		work_done(work, result, work->work_index, work->work_index + 1);
		work->work_index += 1;
		work->work_progress += 1;
	}
	ctx_destroy(&ctx);
}

