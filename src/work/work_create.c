#include "work.h"

static void
	work_create_int(t_work *work, t_state *state,
			t_options *opts, union u_client *client)
{
	work->state = state;
	work->workers = NULL;
	work->count = 0;
	work->work_index = 0;
	work->work_size = 0;
	work->work_progress = 0;
	work->pause = 0;
	work->stop = 0;
	work->paused = 0;
	work->stopped = 0;
	work->data = NULL;
	work->capacity = 0;
	work->opts = opts;
	work->pending = NULL;
	work->pending_size = 0;
	work->pending_capacity = 0;
	work->client = client;
	work->update_stop = 0;
	work->update_flag = 0;
}

void
	work_create(t_work *work, t_state *state,
			t_options *opts, union u_client *client)
{
	work_create_int(work, state, opts, client);
	mutex_init(&work->mtx);
	cond_init(&work->cnd);
	cond_init(&work->update_cnd);
	cond_init(&work->progress_cnd);
	mutex_init(&work->state_mtx);
	mutex_init(&work->update_mtx);
	work_int_create(work);
	work_pause(work);
}
