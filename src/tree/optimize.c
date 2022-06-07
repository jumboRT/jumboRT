#include "tree.h"

#include <stdlib.h>
#include <math.h>

static void
	tree_add(t_tree *tree, t_entity *ent)
{
	tree->list = rt_realloc(tree->list,
			sizeof(*tree->list) * (tree->count + 0),
			sizeof(*tree->list) * (tree->count + 1));
	tree->list[tree->count] = ent;
	tree->count += 1;
}

static void
	tree_split(t_tree *tree, t_vec pos, t_vec dir)
{
	size_t		i;
	t_entity	*ent;
	int			cmp;

	tree->front = tree_new(NULL, 0);
	tree->back = tree_new(NULL, 0);
	tree->plane_pos = pos;
	tree->plane_dir = dir;
	i = 0;
	while (i < tree->count)
	{
		ent = tree->list[i];
		cmp = ent->vt->compare(ent, pos, dir);
		if (cmp >= 0)
			tree_add(tree->front, ent);
		if (cmp <= 0)
			tree_add(tree->back, ent);
		i += 1;
	}
	rt_free(tree->list);
	tree->list = NULL;
	tree->count = 0;
}

/* TODO: maybe don't split empty nodes */
static void
	tree_split3d(t_tree *tree, t_vec pos)
{
	tree_split(tree, pos, vec(1, 0, 0, 0));
	tree_split(tree->front, pos, vec(0, 1, 0, 0));
	tree_split(tree->front->front, pos, vec(0, 0, 1, 0));
	tree_split(tree->front->back, pos, vec(0, 0, 1, 0));
	tree_split(tree->back, pos, vec(0, 1, 0, 0));
	tree_split(tree->back->front, pos, vec(0, 0, 1, 0));
	tree_split(tree->back->back, pos, vec(0, 0, 1, 0));
}

/* TODO: there is an almost duplicate version of this in tree/util.c */
static void 
	tree_bounding_box(t_entity **list, size_t count, t_vec *a1, t_vec *b1)
{
	size_t	index;
	t_vec	a;
	t_vec	b;

	*a1 = vec(HUGE_VAL, HUGE_VAL, HUGE_VAL, 0);
	*b1 = vec(-HUGE_VAL, -HUGE_VAL, -HUGE_VAL, 0);
	index = 0;
	while (index < count)
	{
		list[index]->vt->get_bounds(list[index], &a, &b);
		*a1 = vec_min(a, *a1);
		*b1 = vec_max(b, *b1);
		index++;
	}
}

// TODO: this function does not work properly and is only an estimate rn
FLOAT
	tree_quality(t_tree *tree)
{
	FLOAT	front;
	FLOAT	back;

	if (tree == NULL || tree->list != NULL)
		return (tree->count);
	front = tree_quality(tree->front);
	back = tree_quality(tree->back);
	if (front == 0 || back == 0 || front == HUGE_VAL || back == HUGE_VAL)
		return (HUGE_VAL);
	return (fmax(front, back) / fmin(front, back));
}

t_tree
	*tree_new(t_entity **list, size_t count)
{
	t_tree	*tree;

	tree = rt_malloc(sizeof(*tree));
	tree->list = rt_memdup(list, count * sizeof(*list));
	tree->count = count;
	tree->plane_pos = vec(0, 0, 0, 0);
	tree->plane_dir = vec(0, 0, 1, 0);
	tree->front = NULL;
	tree->back = NULL;
	return (tree);
}

t_tree
	*tree_copy(t_tree *other)
{
	t_tree	*tree;

	if (other == NULL)
		return (NULL);
	tree = tree_new(other->list, other->count);
	tree->plane_pos = other->plane_pos;
	tree->plane_dir = other->plane_dir;
	tree->front = tree_copy(other->front);
	tree->back = tree_copy(other->back);
	return (tree);
}

void
	tree_swap(t_tree *tree, t_tree *other)
{
	t_tree	tmp;

	tmp = *tree;
	*tree = *other;
	*other = tmp;
}

void
	tree_optimizer(t_tree *tree, int depth)
{
	t_tree			*split;
	t_tree			*best;
	t_tree_plane	*planes;
	size_t			count;
	size_t			i;

	if (tree == NULL || tree->count < 2 || depth == 0)
		return ;
	planes = tree_gen_planes(tree->list, tree->count, &count);
	i = 0;
	best = tree_copy(tree);
	while (i < count)
	{
		split = tree_copy(tree);
		tree_split(split, planes[i].pos, planes[i].dir);
		if (tree_quality(split) < tree_quality(best))
			tree_swap(best, split);
		tree_destroy(split);
		i += 1;
	}
	rt_free(planes);
	tree_optimize(best->front, depth - 1);
	tree_optimize(best->back, depth - 1);
	tree_swap(tree, best);
	tree_destroy(best);
}

void
	tree_optimize(t_tree *tree, int depth)
{
	t_vec	a;
	t_vec	b;

	(void) depth;
	tree_bounding_box(tree->list, tree->count, &a, &b);
	tree_split3d(tree, a);
	tree_split3d(tree->front->front->front, b);
}

