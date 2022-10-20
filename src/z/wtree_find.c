#include "z.h"

static void
	zwtree_find_lens_node(unsigned char *lens, struct s_zwtree_node *node)
{
	if (node->left == NULL && node->right == NULL)
		lens[node->code] += 1;
	else
	{
		zwtree_find_lens_node(lens, node->left);
		zwtree_find_lens_node(lens, node->right);
	}
}

void
	zwtree_find_lens(unsigned char *lens, struct s_zwtree_list *lists,
			unsigned int max_len)
{
	size_t	i;

	i = 0;
	while (i < 288)
	{
		lens[i] = 0;
		i += 1;
	}
	i = 0;
	while (i < lists[max_len].size)
	{
		zwtree_find_lens_node(lens, &lists[max_len].nodes[i]);
		i += 1;
	}
	if (lists[max_len].size == 0 && lists[0].size > 0)
		lens[lists[0].nodes[0].code] += 1;
	else if (lists[max_len].size == 0 && lists[0].size == 0)
		lens[0] += 1;
}

void
	zwtree_find_codes(unsigned int *codes, unsigned int *counts,
			unsigned char *lens)
{
	unsigned int	indices[16];
	unsigned int	i;

	indices[0] = 0;
	i = 1;
	while (i < 16)
	{
		indices[i] = (indices[i - 1] + counts[i - 1]) << 1;
		i += 1;
	}
	i = 0;
	while (i < 288)
	{
		if (lens[i] != 0)
			codes[i] = reverse_bits(indices[lens[i]]++, lens[i]);
		i += 1;
	}
}
