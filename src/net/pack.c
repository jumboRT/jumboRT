#include "net.h"

#include "z.h"
#include "util.h"
#include <string.h>

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
	*rt_packfl(void *dst, FLOAT f)
{
	union {
		FLOAT f;
		uint32_t i;
	} conv;
	uint16_t tmp;
	
	conv.f = f;

	tmp = ((conv.i>>16)&0x8000)|
		((((conv.i&0x7f800000)-0x38000000)>>13)&0x7c00)|((conv.i>>13)&0x03ff);
	memcpy(dst, &tmp, sizeof(tmp));
	return ((char *) dst + sizeof(tmp));
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
	*rt_upackfl(void *src, FLOAT *f)
{

	union {
		FLOAT f;
		uint32_t i;
	} conv;
	uint16_t	tmp;

	memcpy(&tmp, src, sizeof(tmp));
	conv.i = ((tmp&0x8000)<<16)|(((tmp&0x7c00)+0x1c000)<<13)|((tmp & 0x03ff)<<13);
	*f = conv.f;
	return ((char *) src + sizeof(tmp));
}

void
	*rt_upackvec(void *src, t_vec *dst)
{
	FLOAT	x;
	FLOAT	y;
	FLOAT	z;

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
	src = rt_upackstr(src, &dst->scene);
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
	dst = rt_packstr(dst, packet.scene);
	return (dst);
}

void
	*rt_packsr(void *dst, struct s_send_results packet)
{
	dst = rt_packu64(dst, packet.seq_id);
	dst = rt_packu64(dst, packet.zsize);
	memcpy(dst, packet.zdata, packet.zsize);
	return ((char *) dst + packet.zsize);
}
