#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <ft_printf.h>
#include "rt.h"
#include "parser.h"

static int
	loop(void *handle)
{
	t_rt_state	*state;

	state = handle;
	win_put(&state->win, &state->img);
	return (0);
}

static int
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
t_vec
	trace_pixel(t_rt_state *state, int x, int y)
{
	const FLOAT	xd = (x - state->img.width / 2.0) / state->img.width * state->scene.camera->fov / 90;
	const FLOAT	yd = (y - state->img.height / 2.0) / state->img.width * state->scene.camera->fov / 90;
	t_ray		ray;
	t_hit		hit;

	ray.pos = state->scene.camera->pos;
	ray.dir = vec_add(state->scene.camera->dir, vec_norm(vec(0.5, xd, yd, 0)));
	if (trace_hit(&state->scene, ray, &hit))
		return (vec_scale(vec_add(hit.normal, vec(1.0, 1.0, 1.0, 0)), 0.5));
	return (vec(0, 0, 0, 0));
}

static void
	setup_events(t_rt_state *state)
{
	win_event_hook(&state->win, RT_WIN_EVENT_CLOSE, rt_exit, state);
}

static void
	start(t_scene *scene)
{
	t_rt_state	state;

	img_create(&state.img, 1920, 1080);
	img_clear(&state.img, 0);
	mutex_init(&state.mtx);
	cond_init(&state.cnd);
	state.scene = *scene;
	state.size = state.img.width * state.img.height;
	state.order = rt_malloc(state.size * sizeof(*state.order));
	rt_random_range(state.order, state.size);
	thread_start(&state);
	win_create(&state.win, loop, state.img.width, state.img.height);
	setup_events(&state);
	win_start(&state.win);
}

int
	main(int argc, char **argv)
{
	char	*error;
	char	*file;
	t_scene	scene;

	if (argc != 2)
	{
		ft_fprintf(STDERR_FILENO, "Usage: %s scene_file\n", argv[0]);
		return (EXIT_FAILURE);
	}
	file = rt_readfile(argv[1], &error);
	if (file == NULL)
	{
		ft_fprintf(STDERR_FILENO, "Error\n%s\n", error);
		exit(EXIT_FAILURE);
	}
	if (rt_scene(&scene, file, &error))
	{
		ft_fprintf(STDERR_FILENO, "Error\n%s\n", error);
		exit(EXIT_FAILURE);
	}
	free(file);
	start(&scene);
	return (EXIT_FAILURE); /* This should never trigger */
}
