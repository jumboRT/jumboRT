#include "z.h"

#include "util.h"

void
	zdecode_code(t_zbuf *ib, unsigned int code, unsigned int *n)
{
	if (code < 16)
		*n = 1;
	else if (code == 16)
		*n = zbuf_read(ib, 2) + 3;
	else if (code == 17)
		*n = zbuf_read(ib, 3) + 3;
	else if (code == 18)
		*n = zbuf_read(ib, 7) + 11;
}

void
	zdecode_length(t_zbuf *ib, unsigned int code, unsigned int *value)
{
	unsigned int	extra_bits;
	unsigned int	start_offset;
	unsigned int	block_offset;

	if (code == 285)
		*value = 258;
	else if (code < 261)
		*value = code - 257 + 3;
	else
	{
		code -= 261;
		extra_bits = code / 4;
		block_offset = (4 << extra_bits) - 4;
		start_offset = (code % 4) << extra_bits;
		*value = start_offset + block_offset + zbuf_read(ib, extra_bits) + 7;
	}
}

void
	zdecode_dist(t_zbuf *ib, unsigned int code, unsigned int *dist)
{
	unsigned int	extra_bits;
	unsigned int	start_offset;
	unsigned int	block_offset;

	rt_assert(code < 30, "zdecode_dist: invalid dist value");
	if (code < 2)
		*dist = code + 1;
	else
	{
		code -= 2;
		extra_bits = code / 2;
		block_offset = (2 << extra_bits) - 2;
		start_offset = (code % 2) << extra_bits;
		*dist = start_offset + block_offset + zbuf_read(ib, extra_bits) + 3;
	}
}
