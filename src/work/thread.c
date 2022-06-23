#include "work.h"

#if defined RT_WORK_THREAD

# ifndef RT_WORK_THREAD_COUNT
#  define RT_WORK_THREAD_COUNT 8
# endif

# ifndef RT_WORK_THREAD_CHUNK_SIZE
#  define RT_WORK_THREAD_CHUNK_SIZE 256
# endif

void
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
			result[i] = work_compute(worker->work->state->world, worker->ctx, begin + i);
			i += 1;
		}
		queue_send(&worker->queue, result, sizeof(*result) * (end - begin));
	}
	return (NULL);
}

void
	work_int_create(t_work *work)
{
	t_context	*ctx;
	size_t		i;

	i = 0;
	while (i < RT_WORK_THREAD_COUNT)
	{
		ctx = rt_malloc(sizeof(*ctx));
		ctx_init(ctx);
		work_add(work, work_start, ctx);
		i += 1;
	}
}

void
	work_int_destroy(t_work *work)
{
	size_t	i;

	i = 0;
	while (i < work->count)
	{
		ctx_destroy(work->workers[i].ctx);
		rt_free(work->workers[i].ctx);
		i += 1;
	}
}

void
	work_int_resume(t_work *work)
{
	(void) work;
}

#endif

