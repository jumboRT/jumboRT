#include "z.h"

static int
	zwtree_calculate_least(struct s_zwtree_node *nodes, unsigned int size, struct s_zwtree_node **a, struct s_zwtree_node **b)
{
	unsigned int	i;

	*a = NULL;
	*b = NULL;
	i = 0;
	while (i < size)
	{
		if (nodes[i].parent == NULL && nodes[i].weight != 0)
		{
			if (*a == NULL || nodes[i].weight < (*a)->weight)
			{
				*b = *a;
				*a = &nodes[i];
			}
			else if (*b == NULL || nodes[i].weight < (*b)->weight)
				*b = &nodes[i];
		}
		i += 1;
	}
	return (*a != NULL && *b != NULL);
}

static void
	zwtree_init_nodes(struct s_zwtree_node *nodes, size_t *weights)
{
	struct s_zwtree_node	*a;
	struct s_zwtree_node	*b;
	unsigned int			i;

	i = 0;
	while (i < 288)
	{
		nodes[i].parent = NULL;
		nodes[i].weight = weights[i];
		i += 1;
	}
	while (zwtree_calculate_least(nodes, i, &a, &b))
	{
		nodes[i].weight = a->weight + b->weight;
		nodes[i].parent = NULL;
		a->parent = &nodes[i];
		b->parent = &nodes[i];
		i += 1;
	}
	if (i == 288 && a != NULL)
		a->parent = &nodes[288];
}

static void
	zwtree_find_lens(unsigned char *lens, struct s_zwtree_node *nodes)
{
	unsigned int			i;
	struct s_zwtree_node	*node;

	i = 0;
	while (i < 288)
	{
		lens[i] = 0;
		node = nodes[i].parent;
		while (node != NULL)
		{
			lens[i] += 1;
			node = node->parent;
		}
		printf("length of %d = %d\n", i, lens[i]);
		i += 1;
	}
}

void
	zwtree_find_codes(unsigned int *codes, unsigned int *counts, unsigned char *lens)
{
	unsigned int	indices[17];
	unsigned int	i;

	indices[0] = 0;
	i = 1;
	while (i < 17)
	{
		indices[i] = (indices[i - 1] + counts[i - 1]) << 1;
		i += 1;
	}
	i = 0;
	while (i < 288)
	{
		if (lens[i] != 0)
			codes[i] = indices[lens[i]]++;
		i += 1;
	}
	
}

void
	zwtree_init(t_zwtree *tree, size_t *weights)
{
	struct s_zwtree_node	nodes[575];
	unsigned int			counts[17];
	unsigned int			i;

	zwtree_init_nodes(nodes, weights);
	zwtree_find_lens(tree->lens, nodes);
	ztree_find_counts(counts, tree->lens, 288);
	zwtree_find_codes(tree->codes, counts, tree->lens);
	tree->count = 0;
	i = 0;
	while (i < 288)
	{
		if (tree->lens[i] != 0)
			tree->count = i + 1;
		i += 1;
	}
}

void
	zwtree_put(t_zwtree *tree, t_zbuf *zb, unsigned int value)
{
	unsigned int	i;

	i = 0;
	while (i < tree->lens[value])
	{
		zbuf_write(zb, 1, (tree->codes[value] >> (tree->lens[value] - i - 1)) & 1);
		i += 1;
	}
}
