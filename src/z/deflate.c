#include "z.h"

#include "util.h"

static void
	z_deflate_fixed(t_ztoken *tokens, size_t count, t_zbuf *ob, t_zwtree *lt, t_zwtree *dt)
{
	struct s_ztoken_data	data;
	t_zwtree				fixed;
	size_t					i;

	zwtree_default(&fixed, &lt);
	i = 0;
	while (i < count)
	{
		if (tokens[i].length <= 1)
			zwtree_put(lt, ob, tokens[i].data.character);
		else
		{
			data = zencode_token(tokens[i]);
			zwtree_put(lt, ob, data.length_code);
			zbuf_write(ob, data.length_extra_bits, data.length_value);
			if (dt == NULL)
				zbuf_write(ob, 5, data.dist_code);
			else
				zwtree_put(dt, ob, data.dist_code);
			zbuf_write(ob, data.dist_extra_bits, data.dist_value);
		}
		i += 1;
	}
	zwtree_put(lt, ob, 256);
}

static void
	z_deflate_plant_trees(t_ztoken *tokens, size_t count, t_zwtree *lt, t_zwtree *dt)
{
	struct s_ztoken_data	data;
	size_t					i;
	size_t					lt_weights[288];
	size_t					dt_weights[288];

	i = 0;
	while (i < 288)
	{
		lt_weights[i] = 0;
		dt_weights[i] = 0;
		i += 1;
	}
	i = 0;
	while (i < count)
	{
		if (tokens[i].length <= 1)
			lt_weights[tokens[i].data.character] += 1;
		else
		{
			data = zencode_token(tokens[i]);
			lt_weights[data.length_code] += 1;
			dt_weights[data.dist_code] += 1;
		}
		i += 1;
	}
	lt_weights[256] += 1;
	zwtree_init(lt, lt_weights);
	zwtree_init(dt, dt_weights);
	dt->count += dt->count == 0;
}

static unsigned int
	z_deflate_analyze_lens(struct s_zwtree_token *tokens, unsigned char *lens, unsigned int size)
{
	struct s_zwtree_token	*begin;
	unsigned int			i;
	unsigned int			j;

	i = 0;
	begin = tokens;
	while (i < size)
	{
		j = 0;
		while (i + j < size && lens[i] == lens[i + j])
			j++;
		tokens += zencode_zwtree_token(tokens, lens[i], j);
		i += j;
	}
	return (tokens - begin);
}

static void
	z_deflate_plant_special_tree(struct s_zwtree_token *tokens, unsigned int size, t_zwtree *ct)
{
	size_t			weights[288];
	unsigned int	i;

	i = 0;
	while (i < 288)
	{
		weights[i] = 0;
		i += 1;
	}
	i = 0;
	while (i < size)
	{
		weights[tokens[i].code] += 1;
		i += 1;
	}
	zwtree_init(ct, weights);
	ct->count = 19;
}

static void
	z_deflate_trees(t_zbuf *ob, t_zwtree *tree, struct s_zwtree_token *tokens, unsigned int count)
{
	unsigned int	i;

	i = 0;
	while (i < tree->count)
		zbuf_write(ob, 3, tree->lens[zfrob(i++)]);
	i = 0;
	while (i < count)
	{
		printf("insert code %d l=%d v=%d\n", tokens[i].code, tree->lens[tokens[i].code], tree->codes[tokens[i].code]);
		zwtree_put(tree, ob, tokens[i].code);
		zbuf_write(ob, tokens[i].extra_bits, tokens[i].value);
		i += 1;
	}
}

static void
	z_deflate_dynamic(t_ztoken *tokens, size_t count, t_zbuf *ob)
{
	struct s_zwtree_token	tree_tokens[320];
	unsigned char			lens[320];
	unsigned int			size;
	t_zwtree				ct;
	t_zwtree				lt;
	t_zwtree				dt;

	z_deflate_plant_trees(tokens, count, &lt, &dt);
	rt_assert(lt.count >= 257, "z_deflate_dynamic: bad lt.count");
	rt_assert(dt.count >= 1, "z_deflate_dynamic: bad dt.count");
	rt_memcpy(lens, lt.lens, lt.count);
	rt_memcpy(lens + lt.count, dt.lens, dt.count);
	size = z_deflate_analyze_lens(tree_tokens, lens, lt.count + dt.count);
	z_deflate_plant_special_tree(tree_tokens, size, &ct);
	rt_assert(ct.count >= 4, "z_deflate_dynamic: bad ct.count");
	zbuf_write(ob, 5, lt.count - 257);
	zbuf_write(ob, 5, dt.count - 1);
	zbuf_write(ob, 4, ct.count - 4);
	z_deflate_trees(ob, &ct, tree_tokens, size);
	z_deflate_fixed(tokens, count, ob, &lt, &dt);
}

void
	*z_deflate(void *src, size_t src_size, size_t *dst_size)
{
	t_zbuf		ib;
	t_zbuf		ob;
	t_ztoken	*tokens;
	size_t		count;

	zbuf_create(&ib, src, src_size);
	zbuf_create(&ob, NULL, 0);
	tokens = lz77_deflate(src, src_size, &count);
	zbuf_write(&ob, 1, 1);
	zbuf_write(&ob, 2, 2);
	z_deflate_dynamic(tokens, count, &ob);
	rt_free(tokens);
	*dst_size = ob.size;
	return (ob.data);
}
