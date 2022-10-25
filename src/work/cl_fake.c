/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cl_fake.c                                      #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:10 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:10 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "cl_impl.h"
#include "work.h"
#include "util.h"

#if !RT_USE_OPENCL

void
	work_int_create_opencl(t_work *work)
{
	(void) work;
	rt_assert(0, "not compiled with opencl support");
}

void
	work_int_destroy_opencl(t_work *work)
{
	(void) work;
	rt_assert(0, "not compiled with opencl support");
}

void
	work_int_resume_opencl(t_work *work)
{
	(void) work;
	rt_assert(0, "not compiled with opencl support");
}

void
	opencl_compile(const char *i, const char *o)
{
	(void) i;
	(void) o;
	rt_assert(0, "not compiled with opencl support");
}

void
	opencl_link(const char **i, const char *o)
{
	(void) i;
	(void) o;
	rt_assert(0, "not compiled with opencl support");
}

#endif
