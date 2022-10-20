#include "z.h"

void
	zwtree_put(t_zwtree *tree, t_zbuf *zb, unsigned int value)
{
	zbuf_write(zb, tree->lens[value], tree->codes[value]);
}

int
	zwtree_node_cmp(const void *a_ptr, const void *b_ptr)
{
	const struct s_zwtree_node	*a;
	const struct s_zwtree_node	*b;

	a = a_ptr;
	b = b_ptr;
	return ((a->weight > b->weight) - (a->weight < b->weight));
}
