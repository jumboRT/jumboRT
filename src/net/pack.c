#include "net.h"

#include "ser.h"
#include "z.h"
#include "util.h"
#include <string.h>
#include <math.h>

void
	*rt_packu64(void *dst, uint64_t i)
{
	unsigned char *buf;

	buf = dst;
	*buf++ = i;
	*buf++ = i >> 8;
	*buf++ = i >> 16;
	*buf++ = i >> 24;
	*buf++ = i >> 32;
	*buf++ = i >> 40;
	*buf++ = i >> 48;
	*buf++ = i >> 56;
	return (buf);
}

void
	*rt_packstr(void *dst, struct s_string str)
{
	dst = rt_packu64(dst, str.len);
	memcpy(dst, str.str, str.len);
	return ((char *) dst + str.len);
}

void
	*rt_packhfl(void *dst, float f)
{
	float		m;
	int			e;
	uint16_t	h;

	m = frexpf(f, &e);
	e += (1 << RT_FLOAT_EXPONENT_BITS) / 2 - 1;
	if (m == 0 || e < 0)
		h = 0;
	else
	{
		m *= 2;
		rt_assert(m >= 1, "rt_packhfl: negative number");
		rt_assert(e < (1 << RT_FLOAT_EXPONENT_BITS), "rt_packhfl: exponent too large");
		h = e << RT_FLOAT_MANTISSA_BITS;
		h |= (uint16_t) ((m - 1) * (1 << RT_FLOAT_MANTISSA_BITS));
	}
	memcpy(dst, &h, sizeof(h));
	return ((char *) dst + sizeof(h));
}

void
	*rt_packfl(void *dst, float f)
{
	memcpy(dst, &f, sizeof(f));
	return ((char *) dst + sizeof(f));
}

void
	*rt_packhvec(void *dst, t_vec vec)
{
	dst = rt_packhfl(dst, x(vec));
	dst = rt_packhfl(dst, y(vec));
	dst = rt_packhfl(dst, z(vec));
	return (dst);
}

void
	*rt_packvec(void *dst, t_vec vec)
{
	dst = rt_packfl(dst, x(vec));
	dst = rt_packfl(dst, y(vec));
	dst = rt_packfl(dst, z(vec));
	return (dst);
}

void
	*rt_packhs(void *dst, struct s_handshake packet, int type)
{
	unsigned char *buf;

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
