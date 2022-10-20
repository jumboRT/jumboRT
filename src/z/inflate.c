#include "z.h"

#include "util.h"

#include <libft.h>

static void
	z_inflate_none(t_zbuf *ib, t_zbuf *ob)
{
	unsigned int	len;
	unsigned int	nlen;

	zbuf_next(ib);
	len = zbuf_read(ib, 16);
	nlen = zbuf_read(ib, 16);
	rt_assert((~len & 0xFFFF) == nlen, "z_inflate_none: check failed");
	zbuf_copy(ob, ib, len);
}

static void
	z_inflate_fixed(t_zbuf *ib, t_zbuf *ob, t_ztree *lt, t_ztree *dt)
{
	unsigned int	value;
	unsigned int	dist;
	t_ztree			fixed;

	ztree_default(&fixed, &lt);
	while (1)
	{
		value = ztree_get(lt, ib);
		if (value == 256)
			break ;
		if (value < 256)
			zbuf_write_aligned(ob, 8, value);
		if (value < 256)
			continue ;
		zdecode_length(ib, value, &value);
		if (dt == NULL)
			dist = zbuf_read(ib, 5);
		else
			dist = ztree_get(dt, ib);
		zdecode_dist(ib, dist, &dist);
		zbuf_repeat(ob, dist, value);
	}
}

static void
	z_inflate_trees(t_zbuf *ib, t_ztree *tree, unsigned char *lens, unsigned int count)
{
	unsigned char	l[19];
	unsigned int	i;
	unsigned int	v;
	unsigned int	n;

	i = 0;
	ft_memset(l, 0, 19);
	while (i < tree->count)
		l[zfrob(i++)] = zbuf_read(ib, 3);
	ztree_init(tree, l, 19);
	i = 0;
	while (i < count)
	{
		v = ztree_get(tree, ib);
		zdecode_code(ib, v, &n);
		rt_assert(i + n <= count, "z_inflate_trees: buffer overflow");
		rt_assert(v != 16 || i > 0, "z_inflate_trees: buffer underflow");
		if (v == 16)
			v = lens[i - 1];
		else if (v == 17 || v == 18)
			v = 0;
		while (n-- > 0)
			lens[i++] = v;
	}
}

static void
	z_inflate_dynamic(t_zbuf *ib, t_zbuf *ob)
{
	unsigned char	lens[320];
	t_ztree			ct;
	t_ztree			lt;
	t_ztree			dt;

	lt.count = zbuf_read(ib, 5) + 257;
	dt.count = zbuf_read(ib, 5) + 1;
	ct.count = zbuf_read(ib, 4) + 4;
	z_inflate_trees(ib, &ct, lens, lt.count + dt.count);
	ztree_init(&lt, lens, lt.count);
	ztree_init(&dt, lens + lt.count, dt.count);
	z_inflate_fixed(ib, ob, &lt, &dt);
}

void
	*z_inflate(void *src, size_t src_size, size_t *dst_size)
{
	t_zbuf			ib;
	t_zbuf			ob;
	unsigned int	final;
	unsigned int	type;

	zbuf_create(&ib, src, src_size);
	zbuf_create(&ob, NULL, 0);
	final = 0;
	while (!final)
	{
		final = zbuf_read(&ib, 1);
		type = zbuf_read(&ib, 2);
		if (type == 0)
			z_inflate_none(&ib, &ob);
		else if (type == 1)
			z_inflate_fixed(&ib, &ob, NULL, NULL);
		else if (type == 2)
			z_inflate_dynamic(&ib, &ob);
		else if (type == 3)
			rt_assert(0, "z_inflate: invalid type");
	}
	*dst_size = ob.size;
	return (ob.data);
}
