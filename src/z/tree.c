#include "z.h"

#include "util.h"

void
	ztree_find_counts(unsigned int *counts, unsigned char *lens,
			unsigned int count)
{
	unsigned int	i;

	i = 0;
	while (i < 16)
	{
		counts[i] = 0;
		i += 1;
	}
	i = 0;
	while (i < count)
	{
		counts[lens[i]] += 1;
		i += 1;
	}
	counts[0] = 0;
}

void
	ztree_find_codes(unsigned short *codes, unsigned int *counts,
			unsigned char *lens, unsigned int count)
{
	unsigned int	indices[16];
	unsigned int	code;
	unsigned int	i;
	unsigned int	j;

	indices[0] = 0;
	i = 1;
	while (i < 16)
	{
		indices[i] = (indices[i - 1] + counts[i - 1]) << 1;
		i += 1;
	}
	i = 0;
	while (i < 32768)
		codes[i++] = count;
	i = 0;
	while (i < count)
	{
		if (lens[i] != 0)
		{
			code = reverse_bits(indices[lens[i]], lens[i]);
			j = 0;
			while (j < (1 << (15 - lens[i])))
				codes[code + (j++ << lens[i])] = i;
			indices[lens[i]] += 1;
		}
		i += 1;
	}
}

void
	ztree_init(t_ztree *tree, unsigned char *lens, unsigned int count)
{
	unsigned int	counts[16];

	rt_memcpy(tree->lens, lens, count);
	ztree_find_counts(counts, lens, count);
	ztree_find_codes(tree->codes, counts, lens, count);
}

unsigned int
	ztree_get(t_ztree *tree, t_zbuf *zb)
{
	unsigned int	peek;
	unsigned int	value;
	int				count;

	peek = zbuf_peek(zb, 15, &count);
	value = tree->codes[peek];
	rt_assert(value < tree->count && tree->lens[value] <= count,
		"ztree_get: too many bits in huffman code");
	zbuf_skip(zb, tree->lens[value]);
	return (value);
}
