#include "rt.h"

static int
	trace_hit(t_scene *scene, t_ray ray, t_hit *hit)
{
	t_hit		tmp;
	size_t		i;
	t_entity	*ent;
	int			did_hit;

	i = 0;
	did_hit = 0;
	hit->t = RT_RAY_LENGTH;
	while (i < scene->count)
	{
		ent = scene->entities[i];
		if (ent->vt->hit != NULL && ent->vt->hit(ent, ray, &tmp))
		{
			if (tmp.t < hit->t)
			{
				did_hit = 1;
				*hit = tmp;
			}
		}
		i += 1;
	}
	return (did_hit);
}

static t_vec
	trace_ray(t_scene *scene, t_ray ray, int depth)
{
	t_hit	hit;
	t_ray	new_ray;
	FLOAT	p;
	FLOAT	cos_theta;
	t_vec	BRDF;
	t_vec	incoming;

	// printf("here ");
	if (depth <= 0)
		return (vec(0, 0, 0, 1.0));
	if (trace_hit(scene, ray, &hit))
	{
		new_ray.pos = hit.pos;
		new_ray.dir = rt_random_hvec(hit.normal);
		p = 1.0 / (2 * RT_PI);
		cos_theta = vec_dot(new_ray.dir, hit.normal);
		BRDF = vec(0.5, 0.5, 0.5, 1.0);
		incoming = trace_ray(scene, new_ray, depth - 1);
		// printf("%f %f %f %f; %f %f %f %f; costheta:%f p:%f\n",
		// 	hit.color.v[X],
		// 	hit.color.v[Y],
		// 	hit.color.v[Z],
		// 	hit.color.v[W],
		// 	incoming.v[X],
		// 	incoming.v[Y],
		// 	incoming.v[Z],
		// 	incoming.v[W],
		// 	cos_theta,
		// 	p
		// 	);
		return (vec_norm(vec_add(hit.color, vec_scale(vec_colormul(BRDF, incoming), cos_theta / p))));
	}
	return (vec(0, 0, 0, 1.0));
}

static t_vec
	trace_ray_diffuse(t_scene *scene, t_ray ray, int depth)
{
	FLOAT	t;
	t_hit	hit;
	t_ray	new_ray;

	if (depth == 0)
		return (vec(0, 0, 0, 0));
	if (trace_hit(scene, ray, &hit))
	{
		new_ray.pos = vec_add(hit.pos, vec_scale(hit.normal, 0.001));
		new_ray.dir = hit.normal;
		return (vec_scale(trace_ray_diffuse(scene, new_ray, depth - 1), 0.5));
	}
	t = 0.5 * (ray.dir.v[Z] + 1.0);
	return (vec_add(vec_scale(vec(1, 1, 1, 0), 1.0 - t), vec_scale(vec(0.5, 0.7, 1.0, 0.0), t)));
}

t_vec
	trace(t_rt_state *state, int x, int y)
{
	t_ray	ray;
	t_vec	color;
	size_t	i;

	i = 0;
	color = vec(0, 0, 0, 0);
	while (i < RT_SAMPLES)
	{
		ray = projection_ray(state,
				x + rt_random_float() - 0.5, y + rt_random_float() - 0.5);
		color = vec_add(color, trace_ray(&state->scene, ray, RT_MAX_DEPTH));
		i += 1;
	}
	return (vec_scale(color, 1.0 / RT_SAMPLES));
}