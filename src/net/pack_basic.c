/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   pack_basic.c                                   #  # #  #   #  ##   ###   */
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
	*rt_packu64(void *dst, t_uint64 i)
{
	unsigned char	*buf;

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
	*rt_packfl(void *dst, float f)
{
	memcpy(dst, &f, sizeof(f));
	return ((char *) dst + sizeof(f));
}

void
	*rt_packhfl(void *dst, float f)
{
	float		m;
	int			e;
	t_uint16	h;

	m = frexpf(f, &e);
	e += (1 << RT_FLOAT_EXPONENT_BITS) / 2 - 1;
	if (m == 0 || e < 0)
		h = 0;
	else
	{
		m *= 2;
		rt_assert(m >= 1, "rt_packhfl: negative number");
		rt_assert(e < (1 << RT_FLOAT_EXPONENT_BITS),
			"rt_packhfl: exponent too large");
		h = e << RT_FLOAT_MANTISSA_BITS;
		h |= (t_uint16)((m - 1) * (1 << RT_FLOAT_MANTISSA_BITS));
	}
	memcpy(dst, &h, sizeof(h));
	return ((char *) dst + sizeof(h));
}
