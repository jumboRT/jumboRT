#include "main.h"
#include "work.h"

#if RT_BONUS

void
	main_update_start(t_work *work)
{
	work_update_start(work);
}

void
	main_update_stop(t_work *work)
{
	work_update_stop(work);
}

#else

void
	main_update_start(t_work *work)
{
	(void) work;
}

void
	main_update_stop(t_work *work)
{
	(void) work;
}

#endif
