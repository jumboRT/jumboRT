#include "tree.h"
#include "rt.h"
#include "ft_printf.h"

#include <stdlib.h>
#include <math.h>

void
	tree_destroy(t_tree *tree)
{
	if (tree != NULL)
	{
		tree_destroy(tree->front);
		tree_destroy(tree->back);
		free(tree->list);
		free(tree);
	}
}

static FLOAT 
	ray_plane_intersect(t_ray ray, t_vec pos, t_vec normal)
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

int
	tree_hit(t_tree *tree, t_ray ray, t_hit *hit, FLOAT max_t)
{
	t_hit		tmp;
	size_t		i;
	size_t		count;
	t_entity	*ent;
	FLOAT		plane_t;
	FLOAT		dot;
	t_hit		hit_front;
	t_hit		hit_back;

	i = 0;
	count = tree->count;
	hit->t = RT_RAY_LENGTH;
	if (tree->list != NULL)
	{
		while (i < count)
		{
			ent = tree->list[i];
			if (ent->vt->hit != NULL && ent->vt->hit(ent, ray, &tmp, 0.001))
				if (tmp.t < hit->t)
					*hit = tmp;
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
	/*tree_hit(tree->front, ray, &hit_front, HUGE_VAL);
	tree_hit(tree->back, ray, &hit_back, HUGE_VAL);
	if (hit_front.t < hit->t)
		*hit = hit_front;
	if (hit_back.t < hit->t)
		*hit = hit_back;*/
	plane_t = ray_plane_intersect(ray, tree->plane_pos, tree->plane_dir);
	dot = vec_dot(vec_sub(ray.pos, tree->plane_pos), tree->plane_dir);
	if (dot > 0)
	{
		if (!tree_hit(tree->front, ray, &hit_front, fmin(plane_t, max_t)) || hit_front.t > plane_t)
		{
			if (plane_t < max_t)
			{
				if (tree_hit(tree->back, ray, &hit_back, max_t))
					if (hit_back.t < hit->t)
						*hit = hit_back;
			}
		}
		if (hit_front.t < hit->t)
			*hit = hit_front;
	}
	else
	{
		if (!tree_hit(tree->back, ray, &hit_back, fmin(plane_t, max_t)) || hit_back.t > plane_t)
		{
			if (plane_t < max_t)
			{
				if (tree_hit(tree->front, ray, &hit_front, max_t))
					if (hit_front.t < hit->t)
						*hit = hit_front;
			}
		}
		if (hit_back.t < hit->t)
			*hit = hit_back;
	}
	return (hit->t < RT_RAY_LENGTH);
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

