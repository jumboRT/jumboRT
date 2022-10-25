/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   size.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:11 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:11 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"

#if RT_BONUS

t_uint64
	rt_sizesr(struct s_send_results packet)
{
	return (32 + packet.zsize);
}

t_uint64
	rt_sizecjr(struct s_cjob_request packet)
{
	return (92 + packet.scene_file.len + packet.scene_key.len);
}
#endif
