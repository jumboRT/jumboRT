/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   server_fake.c                                  #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:09 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:09 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "work.h"
#include "util.h"
#include "net.h"

#if !RT_BONUS

void
	work_int_create_server(t_work *work)
{
	(void) work;
}

void
	work_int_destroy_server(t_work *work)
{
	(void) work;
}

void
	work_int_resume_server(t_work *work)
{
	(void) work;
}

#endif
