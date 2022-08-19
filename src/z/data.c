#include "z.h"

void
	ztree_decode_code(t_zbuf *ib, unsigned int code, unsigned int *n)
{
	if (code < 16)
		*n = 1;
	else if (code == 16)
		*n = zbuf_read(ib, 2);
	else if (code == 17)
		*n = zbuf_read(ib, 3);
	else if (code == 18)
		*n = zbuf_read(ib, 7);
}

/* 1 3 7 15 31 */
/*
void
	ztree_decode_length(t_zbuf *ib, unsigned int code, unsigned int *value)
{
	unsigned int	tmp;
	unsigned int	a, b, c;
	if (code <= 264)
		*value = (code - 267) + 3;
	if (code == 285)
		*value = 258;
	tmp = code - 265;
	a = tmp / 2;
	b = tmp % 2;
	c = a * (
	*value = (((code - 257) >> 2) - 1) + 11;
}
*/

void
	ztree_decode_length(t_zbuf *ib, unsigned int code, unsigned int *value)
{
	unsigned int	extra_bits;
	unsigned int	start_offset;
	unsigned int	block_offset;

	if (code == 285)
		*value = 258;
	if (code < 261)
		*value = code - 257 + 3;
	if (code == 285 || code < 261)
		return ;
	code -= 261;
	extra_bits = code / 4;
	start_offset = (1 << extra_bits) * (code % 4);
	block_offset = (4 << extra_bits);
	*value = start_offset + block_offset + zbuf_read(ib, extra_bits) + 7;
}

void
	ztree_decode_dist(t_zbuf *ib, unsigned int code, unsigned int *dist)
{
	unsigned int	extra_bits;
	unsigned int	start_offset;
	unsigned int	block_offset;

	if (code < 2)
		*dist = code + 1;
	if (code < 2)
		return ;
	code -= 2;
	extra_bits = code / 2;
	start_offset = (1 << extra_bits) * (code % 2);
	block_offset = (2 << extra_bits);
	*dist = start_offset + block_offset + zbuf_read(ib, extra_bits) + 3;
}

void
	ztree_default(t_ztree *tree)
{
	unsigned char	l[288];
	unsigned int	i;

	i = 0;
	while (i < 144)
		l[i++] = 8;
	while (i < 256)
		l[i++] = 9;
	while (i < 280)
		l[i++] = 7;
	while (i < 288)
		l[i++] = 8;
	tree->count = i;
	ztree_init(tree, l);
}
