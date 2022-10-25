/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   unpack_basic.c                                 #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:12 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:12 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "ser.h"
#include "net.h"

#include "util.h"
#include <math.h>
#include <string.h>

void
	*rt_upacku64(void *src, t_uint64 *i)
{
	unsigned char	*buf;

	*i = 0;
	buf = src;
	*i |= (t_uint64)(*buf++);
	*i |= (t_uint64)(*buf++) << 8;
	*i |= (t_uint64)(*buf++) << 16;
	*i |= (t_uint64)(*buf++) << 24;
	*i |= (t_uint64)(*buf++) << 32;
	*i |= (t_uint64)(*buf++) << 40;
	*i |= (t_uint64)(*buf++) << 48;
	*i |= (t_uint64)(*buf++) << 56;
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
	t_uint16	h;

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
