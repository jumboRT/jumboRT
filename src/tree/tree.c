#include "tree.h"
#include "rt.h"
#include "ft_printf.h"
#include "libft.h"

#include <stdlib.h>
#include <math.h>

void
	tree_destroy(t_tree *tree)
{
	if (tree != NULL)
	{
		tree_destroy(tree->front);
		tree_destroy(tree->back);
		rt_free(tree->list);
		rt_free(tree);
	}
}

static FLOAT 
	old_ray_plane_intersect(t_ray ray, t_vec pos, t_vec normal)
{
	FLOAT	dividend;
	FLOAT	divisor;
	FLOAT	t;

	divisor = vec_dot(ray.dir, normal);
	if (float_eq(divisor, 0, 0.00000001))
		return (HUGE_VAL);
	dividend = vec_dot(vec_sub(pos, ray.pos), normal);
//	if ((divisor < 0 && dividend > 0) || (dividend < 0 && divisor > 0))
//		return (HUGE_VAL);
	t = dividend / divisor;
	if (t < 0)
		return (HUGE_VAL);
	return (t);
}

static int
	tree_hit_int(t_thread_ctx *ctx, t_tree *tree, t_ray ray, t_hit *hit, FLOAT max_t)
{
	t_hit		tmp;
	size_t		i;
	t_entity	*ent;
	FLOAT		plane_t;
	FLOAT		dot;
	int			did_hit;

	i = 0;
	tmp.t = RT_RAY_LENGTH;
	if (tree->list != NULL)
	{
		while (i < tree->count)
		{
			ent = tree->list[i];
			if (!ctx->visited[ent->id])
			{
				ctx->visited[ent->id] = 1;
				if (ent->vt->hit != NULL && ent->vt->hit(ent, ray, &tmp, 0.001))
					if (tmp.t < hit->t)
						*hit = tmp;
			}
			i += 1;
		}
		if (hit->t < RT_RAY_LENGTH)
		{
			hit->local_normal = hit->normal;
			if (vec_dot(ray.dir, hit->normal) >= 0)
				hit->local_normal = vec_neg(hit->normal);
		}
		return (hit->t < RT_RAY_LENGTH);
	}
	if (!ray_plane_intersect(ray, tree->plane_pos, tree->plane_dir, &plane_t))
		plane_t = HUGE_VAL;
	dot = vec_dot(vec_sub(ray.pos, tree->plane_pos), tree->plane_dir);
	if (dot > 0)
	{
		did_hit = tree_hit_int(ctx, tree->front, ray, &tmp, fmin(plane_t, max_t));
		if ((!did_hit || tmp.t > plane_t) && plane_t < max_t)
			did_hit = tree_hit_int(ctx, tree->back, ray, &tmp, max_t);
	}
	else
	{
		did_hit = tree_hit_int(ctx, tree->back, ray, &tmp, fmin(plane_t, max_t));
		if ((!did_hit || tmp.t > plane_t) && plane_t < max_t)
			did_hit = tree_hit_int(ctx, tree->front, ray, &tmp, max_t);
	}
	if (tmp.t < hit->t)
		*hit = tmp;
	return (hit->t < RT_RAY_LENGTH);
}

int
	tree_hit(t_thread_ctx *ctx, t_tree *tree, t_ray ray, t_hit *hit, FLOAT max_t)
{
	hit->t = RT_RAY_LENGTH;
	ft_bzero(ctx->visited, ctx->state->scene.count);
	return (tree_hit_int(ctx, tree, ray, hit, max_t));
}

void
	dbg_tree(t_tree *tree, int depth)
{
	if (tree == NULL)
		return ;
	ft_printf("%*s%d\n", depth * 2 + 2, "+ ", (int) tree->count);
	dbg_tree(tree->front, depth + 1);
	dbg_tree(tree->back, depth + 1);
}

