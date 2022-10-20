#include "work.h"

#include "util.h"

#ifndef RT_WORK_THREAD_CHUNK_SIZE
# define RT_WORK_THREAD_CHUNK_SIZE 65536
#endif

static void
	*work_start(void *data)
{
	t_worker	*worker;
	uint64_t	begin;
	uint64_t	end;
	size_t		i;
	t_result	result[RT_WORK_THREAD_CHUNK_SIZE];

	worker = data;
	while (work_sync(worker->work, &begin, &end, RT_WORK_THREAD_CHUNK_SIZE))
	{
		i = 0;
		while (i < end - begin)
		{
			result[i] = work_compute(worker->work->state->world,
					worker->ctx, begin + i);
			i += 1;
		}
		work_send_results(worker, result, begin, end);
	}
	return (NULL);
}

void
	work_int_create_thread(t_work *work)
{
	t_context	*ctx;
	int			i;

	i = 0;
	while (i < work->opts->threads)
	{
		ctx = rt_malloc(sizeof(*ctx));
		ctx_init(ctx);
		work_add(work, work_start, ctx, RT_BACKEND_THREAD);
		i += 1;
	}
}

void
	work_int_destroy_thread(t_work *work)
{
	size_t	i;

	i = 0;
	while (i < work->count)
	{
		if (work->workers[i]->backend == RT_BACKEND_THREAD)
		{
			ctx_destroy(work->workers[i]->ctx);
			rt_free(work->workers[i]->ctx);
		}
		i += 1;
	}
}

void
	work_int_resume_thread(t_work *work)
{
	(void) work;
}
