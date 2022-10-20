#include "z.h"

#include "util.h"
#include "perf.h"

static void
	z_deflate_plant_trees_init(size_t *lt_weights, size_t *dt_weights)
{
	size_t	i;

	i = 0;
	while (i < 288)
	{
		lt_weights[i] = 0;
		dt_weights[i] = 0;
		i += 1;
	}
}

void
	z_deflate_plant_trees(t_ztoken *tokens, size_t count,
			t_zwtree *lt, t_zwtree *dt)
{
	struct s_ztoken_data	data;
	size_t					i;
	size_t					lt_weights[288];
	size_t					dt_weights[288];

	z_deflate_plant_trees_init(lt_weights, dt_weights);
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
	zwtree_init(lt, lt_weights, 15);
	zwtree_init(dt, dt_weights, 15);
	dt->count += dt->count == 0;
}

unsigned int
	z_deflate_analyze_lens(struct s_zwtree_token *tokens,
			unsigned char *lens, unsigned int size)
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

void
	z_deflate_plant_special_tree(struct s_zwtree_token *tokens,
			unsigned int size, t_zwtree *ct)
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
	zwtree_init(ct, weights, 7);
	ct->count = 19;
}
