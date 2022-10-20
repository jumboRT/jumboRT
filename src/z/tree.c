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

static void
	ztree_find_codes_fill(unsigned short *codes, unsigned int cij[3],
			unsigned int count)
{
	while (cij[1] < 32768)
		codes[cij[1]++] = count;
}

void
	ztree_find_codes(unsigned short *codes, unsigned int *counts,
			unsigned char *lens, unsigned int count)
{
	unsigned int	indices[16];
	unsigned int	cij[3];

	indices[0] = 0;
	cij[1] = 1;
	while (cij[1] < 16)
	{
		indices[cij[1]] = (indices[cij[1] - 1] + counts[cij[1] - 1]) << 1;
		cij[1] += 1;
	}
	cij[1] = 0;
	ztree_find_codes_fill(codes, cij, count);
	cij[1] = 0;
	while (cij[1] < count)
	{
		if (lens[cij[1]] != 0)
		{
			cij[0] = reverse_bits(indices[lens[cij[1]]], lens[cij[1]]);
			cij[2] = 0;
			while (cij[2] < (1 << (15 - lens[cij[1]])))
				codes[cij[0] + (cij[2]++ << lens[cij[1]])] = cij[1];
			indices[lens[cij[1]]] += 1;
		}
		cij[1] += 1;
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
