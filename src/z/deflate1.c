/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   deflate1.c                                     #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:13 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 13:09:21 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "z.h"

#include "util.h"
#include "perf.h"

static void
	z_deflate_none(t_zbuf *ib, t_zbuf *ob)
{
	while (ib->index + 65535 < ib->size)
	{
		zbuf_write(ob, 1, 0);
		zbuf_write(ob, 2, 0);
		ob->bit = 0;
		zbuf_write(ob, 16, 65535);
		zbuf_write(ob, 16, 0);
		zbuf_copy(ob, ib, 65535);
		ib->index += 65535;
	}
	zbuf_write(ob, 1, 1);
	zbuf_write(ob, 2, 0);
	ob->bit = 0;
	zbuf_write(ob, 16, ib->size - ib->index);
	zbuf_write(ob, 16, ~(ib->size - ib->index));
	zbuf_copy(ob, ib, ib->size - ib->index);
}

static void
	z_deflate_fixed(t_ztoken *tokens, size_t count, t_zbuf *ob,
			struct s_deflate_fixed_trees t)
{
	struct s_ztoken_data	data;
	t_zwtree				fixed;
	size_t					i;

	zwtree_default(&fixed, &t.lt);
	i = 0;
	while (i < count)
	{
		if (tokens[i].length <= 1)
			zwtree_put(t.lt, ob, tokens[i].data.character);
		else
		{
			data = zencode_token(tokens[i]);
			zwtree_put(t.lt, ob, data.length_code);
			zbuf_write(ob, data.length_extra_bits, data.length_value);
			if (t.dt == NULL)
				zbuf_write(ob, 5, data.dist_code);
			else
				zwtree_put(t.dt, ob, data.dist_code);
			zbuf_write(ob, data.dist_extra_bits, data.dist_value);
		}
		i += 1;
	}
	zwtree_put(t.lt, ob, 256);
}

static void
	z_deflate_trees(t_zbuf *ob, t_zwtree *tree,
			struct s_zwtree_token *tokens, unsigned int count)
{
	unsigned int	i;

	i = 0;
	while (i < tree->count)
	{
		rt_assert(tree->lens[zfrob(i)] < 8,
			"z_deflate_trees: code length length too lengthy");
		zbuf_write(ob, 3, tree->lens[zfrob(i++)]);
	}
	i = 0;
	while (i < count)
	{
		zwtree_put(tree, ob, tokens[i].code);
		zbuf_write(ob, tokens[i].extra_bits, tokens[i].value);
		i += 1;
	}
}

static void
	z_deflate_dynamic(t_ztoken *tokens, size_t count, t_zbuf *ob)
{
	struct s_zwtree_token			tree_tokens[320];
	unsigned char					lens[320];
	unsigned int					size;
	struct s_deflate_dynamic_trees	t;
	struct s_deflate_fixed_trees	fixed_t;

	z_deflate_plant_trees(tokens, count, &t.lt, &t.dt);
	rt_assert(t.lt.count >= 257, "z_deflate_dynamic: bad lt.count");
	rt_assert(t.dt.count >= 1, "z_deflate_dynamic: bad dt.count");
	rt_memcpy(lens, t.lt.lens, t.lt.count);
	rt_memcpy(lens + t.lt.count, t.dt.lens, t.dt.count);
	size = z_deflate_analyze_lens(tree_tokens, lens, t.lt.count + t.dt.count);
	z_deflate_plant_special_tree(tree_tokens, size, &t.ct);
	rt_assert(t.ct.count >= 4, "z_deflate_dynamic: bad ct.count");
	zbuf_write(ob, 5, t.lt.count - 257);
	zbuf_write(ob, 5, t.dt.count - 1);
	zbuf_write(ob, 4, t.ct.count - 4);
	z_deflate_trees(ob, &t.ct, tree_tokens, size);
	fixed_t.lt = &t.lt;
	fixed_t.dt = &t.dt;
	z_deflate_fixed(tokens, count, ob, fixed_t);
}

void
	*z_deflate(void *src, uint64_t src_size, uint64_t *dst_size, int level)
{
	t_zbuf		ib;
	t_zbuf		ob;
	t_ztoken	*tokens;
	size_t		count;

	zbuf_create(&ob, NULL, 0);
	if (level == 0)
	{
		zbuf_create(&ib, src, src_size);
		z_deflate_none(&ib, &ob);
	}
	else
	{
		tokens = lz77_deflate(src, src_size, &count);
		zbuf_write(&ob, 1, 1);
		zbuf_write(&ob, 2, 2);
		z_deflate_dynamic(tokens, count, &ob);
		rt_free(tokens);
	}
	*dst_size = ob.size;
	return (ob.data);
}
