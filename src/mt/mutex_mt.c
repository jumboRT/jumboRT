#include "mt.h"
#include "util.h"


#include <errno.h>

#ifdef RT_MT

void
	mutex_init(t_mutex *mtx)
{
	int	ret;

	ret = pthread_mutex_init(mtx, NULL);
	rt_assert(!ret, "mutex init failed");
}

void
	mutex_destroy(t_mutex *mtx)
{
	int	ret;

	ret = pthread_mutex_destroy(mtx);
	rt_assert(!ret, "mutex destroy failed");
}

void
	mutex_lock(t_mutex *mtx)
{
	int	ret;

	ret = pthread_mutex_lock(mtx);
	rt_assert(!ret, "mutex lock failed");
}

void
	mutex_unlock(t_mutex *mtx)
{
	int	ret;

	ret = pthread_mutex_unlock(mtx);
	rt_assert(!ret, "mutex unlock failed");
}

#endif
