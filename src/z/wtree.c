#include "z.h"

#include "util.h"

static void
	zwtree_init_nodes(struct s_zwtree_list *lists, size_t *weights,
			unsigned int max_len)
{
	size_t	i;

	i = 0;
	while (i <= max_len)
	{
		lists[i].size = 0;
		i += 1;
	}
	i = 0;
	while (i < 288)
	{
		if (weights[i] != 0)
		{
			lists[0].nodes[lists[0].size].left = NULL;
			lists[0].nodes[lists[0].size].right = NULL;
			lists[0].nodes[lists[0].size].weight = weights[i];
			lists[0].nodes[lists[0].size].code = i;
			lists[0].size += 1;
		}
		i += 1;
	}
	view_sort_array(lists[0].nodes, lists[0].size, sizeof(lists[0].nodes[0]),
		zwtree_node_cmp);
}

static void
	zwtree_package_merge_loop(struct s_zwtree_list *lists, unsigned int *l,
			size_t *i, size_t *j)
{
	if (*j / 2 < lists[*l].size / 2 && lists[*l].nodes[*j + 0].weight
		+ lists[*l].nodes[*j + 1].weight < lists[0].nodes[*i].weight)
	{
		lists[*l + 1].nodes[lists[*l + 1].size].left = &lists[*l].nodes[*j + 0];
		lists[*l + 1].nodes[lists[*l + 1].size].right
			= &lists[*l].nodes[*j + 1];
		lists[*l + 1].nodes[lists[*l + 1].size].weight
			= lists[*l].nodes[*j + 0].weight + lists[*l].nodes[*j + 1].weight;
		lists[*l + 1].nodes[lists[*l + 1].size].code = 0;
		lists[*l + 1].size += 1;
		*j += 2;
	}
	else
	{
		lists[*l + 1].nodes[lists[*l + 1].size].left = NULL;
		lists[*l + 1].nodes[lists[*l + 1].size].right = NULL;
		lists[*l + 1].nodes[lists[*l + 1].size].weight
			= lists[0].nodes[*i].weight;
		lists[*l + 1].nodes[lists[*l + 1].size].code = lists[0].nodes[*i].code;
		lists[*l + 1].size += 1;
		*i += 1;
	}
}

static void
	zwtree_package_merge(struct s_zwtree_list *lists, unsigned int max_len)
{
	unsigned int	l;
	size_t			i;
	size_t			j;

	l = 0;
	while (l < max_len)
	{
		i = 0;
		j = 0;
		while (i < lists[0].size && l != max_len - 1)
			zwtree_package_merge_loop(lists, &l, &i, &j);
		while (j / 2 < lists[l].size / 2)
		{
			lists[l + 1].nodes[lists[l + 1].size].left = &lists[l].nodes[j + 0];
			lists[l + 1].nodes[lists[l + 1].size].right
				= &lists[l].nodes[j + 1];
			lists[l + 1].nodes[lists[l + 1].size].weight
				= lists[l].nodes[j + 0].weight + lists[l].nodes[j + 1].weight;
			lists[l + 1].nodes[lists[l + 1].size].code = 0;
			lists[l + 1].size += 1;
			j += 2;
		}
		l += 1;
	}
}

void
	zwtree_init(t_zwtree *tree, size_t *weights, unsigned int max_len)
{
	struct s_zwtree_list	lists[16];
	unsigned int			counts[16];
	unsigned int			i;

	zwtree_init_nodes(lists, weights, max_len);
	zwtree_package_merge(lists, max_len);
	zwtree_find_lens(tree->lens, lists, max_len);
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
