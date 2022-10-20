#if RT_BONUS
# include "net.h"
# include "ser.h"
# include <string.h>

void
	*rt_packhs(void *dst, struct s_handshake packet, int type)
{
	unsigned char	*buf;

	buf = dst;
	*buf++ = packet.client_type;
	buf = rt_packu64(buf, packet.protover);
	if (type == RT_WORKER)
		buf = rt_packu64(buf, packet.req_jobs);
	return (buf);
}

void
	*rt_packsw(void *dst, struct s_send_work packet)
{
	dst = rt_packu64(dst, packet.begin);
	dst = rt_packu64(dst, packet.end);
	return (dst);
}

void
	*rt_packcjr(void *dst, struct s_cjob_request packet)
{
	dst = rt_packu64(dst, packet.width);
	dst = rt_packu64(dst, packet.height);
	dst = rt_packvec(dst, packet.cam_pos);
	dst = rt_packvec(dst, packet.cam_dir);
	dst = rt_packfl(dst, packet.cam_fov);
	dst = rt_packfl(dst, packet.cam_focus);
	dst = rt_packfl(dst, packet.cam_blur);
	dst = rt_packstr(dst, packet.scene_file);
	dst = rt_packstr(dst, packet.scene_key);
	dst = rt_packu64(dst, packet.render_mode);
	dst = rt_packu64(dst, packet.batch_size);
	dst = rt_packu64(dst, packet.trace_batch_size);
	return (dst);
}

void
	*rt_packsr(void *dst, struct s_send_results packet)
{
	dst = rt_packu64(dst, packet.seq_id);
	dst = rt_packu64(dst, packet.zsize);
	dst = rt_packu64(dst, packet.begin);
	dst = rt_packu64(dst, packet.end);
	memcpy(dst, packet.zdata, packet.zsize);
	return ((char *) dst + packet.zsize);
}

#endif
