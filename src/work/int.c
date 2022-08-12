#include "work.h"

void
	work_int_create(t_work *work)
{
	if (work->backends & RT_BACKEND_SINGLE)
		work_int_create_single(work);
	if (work->backends & RT_BACKEND_THREAD)
		work_int_create_thread(work);
	if (work->backends & RT_BACKEND_OPENCL)
		work_int_create_opencl(work);
}

void
	work_int_destroy(t_work *work)
{
	if (work->backends & RT_BACKEND_SINGLE)
		work_int_destroy_single(work);
	if (work->backends & RT_BACKEND_THREAD)
		work_int_destroy_thread(work);
	if (work->backends & RT_BACKEND_OPENCL)
		work_int_destroy_opencl(work);
}

void
	work_int_resume(t_work *work)
{
	if (work->backends & RT_BACKEND_SINGLE)
		work_int_resume_single(work);
	if (work->backends & RT_BACKEND_THREAD)
		work_int_resume_thread(work);
	if (work->backends & RT_BACKEND_OPENCL)
		work_int_resume_opencl(work);
}

