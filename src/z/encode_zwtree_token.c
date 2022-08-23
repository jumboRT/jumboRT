#include "z.h"

static unsigned int
	zwtt_write(struct s_zwtree_token *data, unsigned int code, unsigned int value)
{
	printf("zwtt %d %d\n", code, value);
	data->code = code;
	if (code == 16)
	{
		data->extra_bits = 2;
		data->value = value - 3;
	}
	else if (code == 17)
	{
		data->extra_bits = 3;
		data->value = value - 3;
	}
	else if (code == 18)
	{
		data->extra_bits = 7;
		data->value = value - 11;
	}
	else
	{
		data->extra_bits = 0;
		data->value = 0;
	}
	return (1);
}

unsigned int
	zwtt_write_part(struct s_zwtree_token *data, unsigned int code, unsigned int max, unsigned int *count)
{
	unsigned int	n;

	n = *count;
	if (n > max)
		n = max;
	*count -= n;
	return (zwtt_write(data, code, n));
}

unsigned int
	zencode_zwtree_token(struct s_zwtree_token *data, unsigned int length, unsigned int count)
{
	unsigned int	i;

	i = 0;
	if (length != 0)
	{
		i += zwtt_write(data + i, length, 0);
		count -= 1;
		while (count >= 3)
			i += zwtt_write_part(data + i, 16, 6, &count);
	}
	else
	{
		while (count >= 11)
			i += zwtt_write_part(data + i, 18, 138, &count);
		while (count >= 3)
			i += zwtt_write_part(data + i, 17, 10, &count);
	}
	while (count >= 1)
	{
		i += zwtt_write(data + i, length, 0);
		count -= 1;
	}
	return (i);
}

