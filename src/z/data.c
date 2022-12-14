/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   data.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:13 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:13 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "z.h"

static void
	default_lengths(unsigned char *l)
{
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
}

unsigned int
	zfrob(unsigned int x)
{
	static const unsigned int	frobnication_table[] = {
		16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
	};

	return (frobnication_table[x]);
}

void
	ztree_default(t_ztree *tree, t_ztree **dst)
{
	unsigned int	counts[16];

	if (dst == NULL || *dst == NULL)
	{
		default_lengths(tree->lens);
		ztree_find_counts(counts, tree->lens, 288);
		ztree_find_codes(tree->codes, counts, tree->lens, 288);
		if (dst != NULL)
			*dst = tree;
	}
}

void
	zwtree_default(t_zwtree *tree, t_zwtree **dst)
{
	unsigned int	counts[16];

	if (dst == NULL || *dst == NULL)
	{
		default_lengths(tree->lens);
		ztree_find_counts(counts, tree->lens, 288);
		zwtree_find_codes(tree->codes, counts, tree->lens);
		if (dst != NULL)
			*dst = tree;
	}
}
