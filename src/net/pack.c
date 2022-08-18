#include "net.h"

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
	memcpy(dst, &f, sizeof(f));
	return ((char *) dst + sizeof(f));
}

void
	*rt_packvec(void *dst, t_vec vec)
{
	dst = rt_packfl(dst, x(vec));
	dst = rt_packfl(dst, y(vec));
	dst = rt_packfl(dst, z(vec));
	dst = rt_packfl(dst, w(vec));
	return (dst);
}

void
	*rt_packres(void *dst, t_result result)
{
	dst = rt_packu64(dst, result.index);
	dst = rt_packvec(dst, result.color);
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
	memcpy(f, src, sizeof(*f));
	return ((char *) src + sizeof(*f));
}

void
	*rt_upackvec(void *src, t_vec *dst)
{
	FLOAT	x;
	FLOAT	y;
	FLOAT	z;
	FLOAT	w;

	src = rt_upackfl(src, &x);
	src = rt_upackfl(src, &y);
	src = rt_upackfl(src, &z);
	src = rt_upackfl(src, &w);
	*dst = vec(x, y, z, w);
	return (src);
}

void
	*rt_upackres(void *src, t_result *result)
{
	src = rt_upacku64(src, &result->index);
	src = rt_upackvec(src, &result->color);
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
	size_t	idx;

	src = rt_upacku64(src, &dst->seq_id);
	src = rt_upacku64(src, &dst->count);
	dst->results = rt_malloc(dst->count * sizeof(*dst->results));
	idx = 0;
	while (idx < dst->count)
	{
		src = rt_upackres(src, &dst->results[idx]);
		++idx;
	}
	return (src);
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
	uint64_t	idx;

	dst = rt_packu64(dst, packet.seq_id);
	dst = rt_packu64(dst, packet.count);
	idx = 0;
	while (idx < packet.count)
	{
		dst = rt_packres(dst, packet.results[idx]);
		++idx;
	}
	return (dst);
}
