#include "z.h"

#include "util.h"

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

	if (lt == NULL)
		ztree_default(&fixed);
	if (lt == NULL)
		lt = &fixed;
	value = ztree_get(lt, ib);
	while (value != 256)
	{
		if (value < 256)
			zbuf_write(ob, 8, value);
		if (value < 256)
			continue ;
		if (dt == NULL)
			dist = zbuf_read(ib, 5);
		else
			dist = ztree_get(dt, ib);
		rt_assert(dist < 30, "z_inflate_fixed: invalid dist value");
		ztree_decode_length(ib, value, &value);
		ztree_decode_dist(ib, dist, &dist);
		zbuf_repeat(ob, dist, value);
		value = ztree_get(lt, ib);
	}
}

static void
	z_inflate_tree(t_zbuf *ib, t_ztree *tree, t_ztree *ct)
{
	unsigned char	l[286];
	unsigned int	i;
	unsigned int	v;
	unsigned int	n;

	i = 0;
	while (i < tree->count)
	{
		n = 1;
		if (ct == NULL)
			v = zbuf_read(ib, 3);
		else
		{
			v = ztree_get(ct, ib);
			ztree_decode_code(ib, v, &n);
			rt_assert(v != 16 || i > 0, "z_inflate_tree: buffer underflow");
			if (v == 16)
				v = l[i - 1];
			else if (v == 17 || v == 18)
				v = 0;
		}
		while (n-- > 0)
			l[i++] = v;
	}
	ztree_init(tree, l);
}

static void
	z_inflate_dynamic(t_zbuf *ib, t_zbuf *ob)
{
	t_ztree			ct;
	t_ztree			lt;
	t_ztree			dt;

	lt.count = zbuf_read(ib, 5) + 257;
	dt.count = zbuf_read(ib, 5) + 1;
	ct.count = zbuf_read(ib, 4) + 4;
	z_inflate_tree(ib, &ct, NULL);
	z_inflate_tree(ib, &lt, &ct);
	z_inflate_tree(ib, &dt, &ct);
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
			rt_assert(0, "z_inflate: invalid btype");
	}
	*dst_size = ob.size;
	return (ob.data);
}

