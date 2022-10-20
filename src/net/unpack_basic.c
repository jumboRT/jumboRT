#include "ser.h"
#include "net.h"

#include "util.h"
#include <math.h>
#include <string.h>

void
	*rt_upacku64(void *src, uint64_t *i)
{
	unsigned char	*buf;

	*i = 0;
	buf = src;
	*i |= (uint64_t)(*buf++);
	*i |= (uint64_t)(*buf++) << 8;
	*i |= (uint64_t)(*buf++) << 16;
	*i |= (uint64_t)(*buf++) << 24;
	*i |= (uint64_t)(*buf++) << 32;
	*i |= (uint64_t)(*buf++) << 40;
	*i |= (uint64_t)(*buf++) << 48;
	*i |= (uint64_t)(*buf++) << 56;
	return (buf);
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
		m = (h & ((1 << RT_FLOAT_MANTISSA_BITS) - 1))
			/ (float)(1 << RT_FLOAT_MANTISSA_BITS) + 1;
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
