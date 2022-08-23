#include "z.h"

#include "util.h"

static unsigned int
	clz(unsigned int x)
{
	unsigned int i;

	i = 0;
	while (x >> i)
		i += 1;
	return (32 - i);
}

void
	zencode_length(unsigned int length, unsigned int *code_out, unsigned int *extra_bits_out, unsigned int *value_out)
{
	unsigned int	code;
	unsigned int	extra_bits;
	unsigned int	start_offset;
	unsigned int	block_offset;

	rt_assert(length >= 3 && length <= 258, "zencode_length: out of range");
	extra_bits = 0;
	if (length == 258)
		code = 285;
	else if (length < 7)
		code = length - 3 + 257;
	else
	{
		length -= 7;
		extra_bits = 31 - clz(length / 4 + 1);
		block_offset = (4 << extra_bits) - 4;
		start_offset = (length - block_offset) >> extra_bits;
		code = extra_bits * 4 + start_offset + 261;
		if (value_out != NULL)
			*value_out = length - block_offset - (start_offset << extra_bits);
	}
	if (code_out != NULL)
		*code_out = code;
	if (extra_bits_out != NULL)
		*extra_bits_out = extra_bits;
}

void
	zencode_dist(unsigned int dist, unsigned int *code_out, unsigned int *extra_bits_out, unsigned int *value_out)
{
	unsigned int	code;
	unsigned int	extra_bits;
	unsigned int	start_offset;
	unsigned int	block_offset;

	rt_assert(dist >= 1 && dist <= 32768, "zencode_dist: out of range");
	extra_bits = 0;
	if (dist < 3)
		code = dist - 1;
	else
	{
		dist -= 3;
		extra_bits = 31 - clz(dist / 2 + 1);
		block_offset = (2 << extra_bits) - 2;
		start_offset = (dist - block_offset) >> extra_bits;
		code = extra_bits * 2 + start_offset + 2;
		if (value_out != NULL)
			*value_out = dist - block_offset - (start_offset << extra_bits);
	}
	if (code_out != NULL)
		*code_out = code;
	if (extra_bits_out != NULL)
		*extra_bits_out = extra_bits;
}

struct s_ztoken_data
	zencode_token(t_ztoken token)
{
	struct s_ztoken_data	data;

	zencode_length(token.length, &data.length_code,
			&data.length_extra_bits, &data.length_value);
	zencode_dist(token.data.distance, &data.dist_code,
			&data.dist_extra_bits, &data.dist_value);
	return (data);
}

