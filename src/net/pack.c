#include "net.h"

#include "z.h"
#include "util.h"
#include <string.h>
#include <math.h>

#define RT_FLOAT_MANTISSA_BITS 8
#define RT_FLOAT_EXPONENT_BITS 8

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
	*rt_upackstr(void *src, struct s_string *string)
{
	src = rt_upacku64(src, &string->len);
	string->str = rt_malloc(string->len + 1);
	memcpy(string->str, src, string->len);
	string->str[string->len] = '\0';
	src = (char *) src + string->len;
	return (src);
}

void
	*rt_upackhfl(void *src, float *f)
{
	float		m;
	int			e;
	uint16_t	h;

	memcpy(&h, src, sizeof(h));
	if (h == 0)
		*f = 0;
	else
	{
		m = (h & ((1 << RT_FLOAT_MANTISSA_BITS) - 1)) / (float) (1 << RT_FLOAT_MANTISSA_BITS) + 1;
		e = h >> RT_FLOAT_MANTISSA_BITS;
		e -= (1 << RT_FLOAT_EXPONENT_BITS) / 2 - 1;
		m /= 2;
		*f = ldexpf(m, e);
	}
	return ((char *) src + sizeof(h));
}

void
	*rt_upackfl(void *src, float *f)
{
	memcpy(f, src, sizeof(*f));
	return ((char *) src + sizeof(*f));
}

void
	*rt_upackhvec(void *src, t_vec *dst)
{
	float	x;
	float	y;
	float	z;

	src = rt_upackhfl(src, &x);
	src = rt_upackhfl(src, &y);
	src = rt_upackhfl(src, &z);
	*dst = vec(x, y, z, 0.0);
	return (src);
}

void
	*rt_upackvec(void *src, t_vec *dst)
{
	float	x;
	float	y;
	float	z;

	src = rt_upackfl(src, &x);
	src = rt_upackfl(src, &y);
	src = rt_upackfl(src, &z);
	*dst = vec(x, y, z, 0.0);
	return (src);
}

void
	*rt_upacku64(void *src, uint64_t *i)
{
	unsigned char *buf;
	
	*i = 0;
	buf = src;
	*i |= (uint64_t) *buf++;
	*i |= (uint64_t) (*buf++) << 8;
	*i |= (uint64_t) (*buf++) << 16;
	*i |= (uint64_t) (*buf++) << 24;
	*i |= (uint64_t) (*buf++) << 32;
	*i |= (uint64_t) (*buf++) << 40;
	*i |= (uint64_t) (*buf++) << 48;
	*i |= (uint64_t) (*buf++) << 56;
	return (buf);
}

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

void
	*rt_packhs(void *dst, struct s_handshake packet)
{
	unsigned char *buf;

	buf = dst;
	*buf++ = packet.client_type;
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
