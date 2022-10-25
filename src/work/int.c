/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   int.c                                          #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:09 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:09 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "work.h"

void
	work_int_create(t_work *work)
{
	if (work->opts->backends & RT_BACKEND_SINGLE)
		work_int_create_single(work);
	if (work->opts->backends & RT_BACKEND_THREAD)
		work_int_create_thread(work);
	if (work->opts->backends & RT_BACKEND_OPENCL)
		work_int_create_opencl(work);
	if (work->opts->backends & RT_BACKEND_SERVER)
		work_int_create_server(work);
}

void
	work_int_destroy(t_work *work)
{
	if (work->opts->backends & RT_BACKEND_SINGLE)
		work_int_destroy_single(work);
	if (work->opts->backends & RT_BACKEND_THREAD)
		work_int_destroy_thread(work);
	if (work->opts->backends & RT_BACKEND_OPENCL)
		work_int_destroy_opencl(work);
	if (work->opts->backends & RT_BACKEND_SERVER)
		work_int_destroy_server(work);
}

void
	work_int_resume(t_work *work)
{
	if (work->opts->backends & RT_BACKEND_SINGLE)
		work_int_resume_single(work);
	if (work->opts->backends & RT_BACKEND_THREAD)
		work_int_resume_thread(work);
	if (work->opts->backends & RT_BACKEND_OPENCL)
		work_int_resume_opencl(work);
	if (work->opts->backends & RT_BACKEND_SERVER)
		work_int_resume_server(work);
}
