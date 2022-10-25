/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   unpack_packet.c                                #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:12 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:12 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "net.h"
#include "ser.h"
#include "util.h"
#include <string.h>

#if RT_BONUS

void
	*rt_upacksjr(void *src, struct s_sjob_request *dst)
{
	src = rt_upacku64(src, &dst->seq_id);
	src = rt_upacku64(src, &dst->width);
	src = rt_upacku64(src, &dst->height);
	src = rt_upackvec(src, &dst->cam_pos);
	src = rt_upackvec(src, &dst->cam_dir);
	src = rt_upackfl(src, &dst->cam_fov);
	src = rt_upackfl(src, &dst->cam_focus);
	src = rt_upackfl(src, &dst->cam_blur);
	src = rt_upackstr(src, &dst->scene_file);
	src = rt_upackstr(src, &dst->scene_key);
	src = rt_upacku64(src, &dst->render_mode);
	src = rt_upacku64(src, &dst->batch_size);
	src = rt_upacku64(src, &dst->trace_batch_size);
	return (src);
}

void
	*rt_upacksw(void *src, struct s_send_work *dst)
{
	src = rt_upacku64(src, &dst->begin);
	src = rt_upacku64(src, &dst->end);
	return (src);
}

void
	*rt_upacksr(void *src, struct s_send_results *dst)
{
	src = rt_upacku64(src, &dst->seq_id);
	src = rt_upacku64(src, &dst->zsize);
	src = rt_upacku64(src, &dst->begin);
	src = rt_upacku64(src, &dst->end);
	dst->zdata = rt_malloc(dst->zsize);
	memcpy(dst->zdata, src, dst->zsize);
	return ((char *) src + dst->zsize);
}

#endif
