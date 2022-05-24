#include "rt.h"


#include <stdio.h>
#include <stdlib.h>

/* TODO: only apply the minimum T for hitting the same object twice */
/* TODO: or come up with a different method to do the same thing */
static int
	trace_hit(t_scene *scene, t_ray ray, t_hit *hit)
{
	t_hit		tmp;
	size_t		i;
	t_entity	*ent; int			did_hit;
	i = 0;
	did_hit = 0;
	hit->t = RT_RAY_LENGTH;
	while (i < scene->count)
	{
		ent = scene->entities[i];
		if (ent->vt->hit != NULL && ent->vt->hit(ent, ray, &tmp))
		{
			if (tmp.t < hit->t && tmp.t > 0.001)
			{
				did_hit = 1;
				*hit = tmp;
			}
		}
		i += 1;
	}
	return (did_hit);
}

/*
static t_vec
	trace_ray(t_thread_ctx *ctx, t_scene *scene, t_ray ray, int depth)
{
	t_hit	hit;
	t_ray	new_ray;
	FLOAT	p;
	FLOAT	cos_theta;
	t_vec	BRDF;
	t_vec	incoming;

	// printf("here ");
	if (depth <= 0)
		return (vec(0.0, 0.0, 0.0, 0.0));
	if (trace_hit(scene, ray, &hit))
	{
		p = 1.0 / (RT_2PI);
//		BRDF = vec_scale(vec(0.5, 0.5, 0.5, 1.0), 1.0 / RT_PI);
		BRDF = vec_scale(vec(5.0, 5.0, 5.0, 1.0), 1);
		new_ray.pos = vec_add(hit.pos, vec_scale(hit.normal, 0.01));
		new_ray.dir = rt_random_hvec(&ctx->seed, hit.normal);
		cos_theta = vec_dot(new_ray.dir, hit.normal);
		incoming = trace_ray(ctx, scene, new_ray, depth - 1);
		return (vec_clamp(vec_add(vec_scale(hit.color, 0.5), vec_scale(color_mul(BRDF, incoming), cos_theta / p)), 0.0, 1.0));
	}
	return (vec(0.0, 0.0, 0.0, 0.0));
}
*/

/* TODO: offset from start position so it doesn't intersect itself again */
static t_vec
	trace_ray_diffuse(t_thread_ctx *ctx, t_scene *scene, t_ray ray, int depth)
{
	FLOAT		t;
	t_hit		hit;
	t_scatter	scatter;

	if (depth == 0)
		return (vec(0, 0, 0, 0));
	if (trace_hit(scene, ray, &hit))
	{
		if (hit.mat->vt->scatter(hit.mat, &hit, &scatter, ctx))
			return (color_mul(scatter.attenuation, trace_ray_diffuse(ctx, scene, scatter.scattered, depth - 1)));
		return (vec(0, 0, 0, 0));
	}
	t = 0.5 * (ray.dir.v[Z] + 1.0);
	return (vec_add(vec_scale(vec(1, 1, 1, 0), 1.0 - t), vec_scale(vec(0.5, 0.7, 1.0, 0.0), t)));
}

t_vec
	trace(t_thread_ctx *ctx, t_rt_state *state, int x, int y)
{
	t_ray	ray;
	t_vec	color;
	size_t	i;

	i = 0;
	color = vec(0, 0, 0, 0);
	while (i < RT_SAMPLES)
	{
		ray = project_ray(state,
				x + rt_random_float(&ctx->seed), y + rt_random_float(&ctx->seed));
		color = vec_add(color, trace_ray_diffuse(ctx, &state->scene, ray, RT_MAX_DEPTH));
		i += 1;
	}
	return (vec_scale(color, 1.0 / RT_SAMPLES));
}
