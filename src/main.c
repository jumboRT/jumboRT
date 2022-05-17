#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "rt.h"

#ifndef RT_MT
# define RT_THREADS 1
# define RT_ONESHOT 1
#else
# define RT_ONESHOT 0
# ifndef RT_THREADS
#  define RT_THREADS 8
# endif
#endif

int
	loop(void *handle)
{
	t_rt_state	*state;

	state = handle;
	win_put(&state->win, &state->img);
	return (0);
}

int
	trace_hit(t_scene *scene, t_ray ray, t_hit *hit)
{
	size_t		i;
	t_entity	*ent;

	i = 0;
	while (i < scene->count)
	{
		ent = scene->entities[i];
		if (ent->vt != NULL && ent->vt->hit(ent, ray, hit))
			return (1);
		i += 1;
	}
	return (0);
}

/* TODO: fix the hacky projection */
void
	trace_pixel(t_rt_state *state, int x, int y)
{
	const FLOAT	xd = (x - state->img.width / 2.0) / state->img.width * state->scene.camera->fov / 90;
	const FLOAT	yd = (y - state->img.height / 2.0) / state->img.width * state->scene.camera->fov / 90;
	t_ray		ray;
	t_hit		hit;

	ray.pos = state->scene.camera->pos;
	ray.dir = vec_add(state->scene.camera->dir, vec_norm(vec(0.5, xd, yd, 0)));
	if (trace_hit(&state->scene, ray, &hit))
		img_putf(&state->img, x, y, vec_scale(vec_add(hit.normal, vec(1.0, 1.0, 1.0, 0)), 0.5));
}

void
	*start(void *arg)
{
	t_rt_state	*state;
	size_t		i;
	size_t		n;

	state = arg;
	mutex_lock(&state->mtx);
	n = state->img.width * state->img.height;
	while (state->running)
	{
		while (state->idx < n)
		{
			i = state->idx;
			state->idx += 1;
			mutex_unlock(&state->mtx);
			trace_pixel(state, i % state->img.width, i / state->img.width);
			mutex_lock(&state->mtx);
		}
		if (RT_ONESHOT)
			state->running = 0;
		else
			cond_wait(&state->cnd, &state->mtx);
	}
	mutex_unlock(&state->mtx);
	return (NULL);
}

void
	run(t_rt_state *state)
{
	t_thread	threads[RT_THREADS];
	size_t		i;

	i = 0;
	while (i < RT_THREADS)
	{
		thread_create(&threads[i], start, state);
		i += 1;
	}
}

void
	init_scene(t_scene *scene)
{
	t_sphere	*sphere;

	scene->camera = rt_malloc(sizeof(*scene->camera));
	scene->camera->base.vt = NULL;
	scene->camera->pos = vec(0, 0, 0, 0);
	scene->camera->dir = vec(1, 0, 0, 0);
	scene->camera->fov = 90;
	scene->entities = rt_malloc(sizeof(*scene->entities));
	sphere = rt_malloc(sizeof(*sphere));
	sphere->base.vt = sphere_vt();
	sphere->pos = vec(1, 0, 0, 0);
	sphere->diameter = 0.5;
	scene->entities[0] = (t_entity *) sphere;
	scene->count = 1;
}

int
	main(void)
{
	t_rt_state	state;

	img_create(&state.img, 1920, 1080);
	img_clear(&state.img, 0);
	mutex_init(&state.mtx);
	cond_init(&state.cnd);
	state.idx = 0;
	state.running = 1;
	init_scene(&state.scene);
	run(&state);
	win_create(&state.win, loop, state.img.width, state.img.height);
	win_start(&state.win);
	return (EXIT_SUCCESS);
}
