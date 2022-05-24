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

static void
	setup_events(t_rt_state *state)
{
	win_event_hook(&state->win, RT_WIN_EVENT_CLOSE, rt_exit, state);
	win_event_hook(&state->win, RT_WIN_EVENT_KEY_DOWN, rt_key_down, state);
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
	state.use_conic = 0;
	rt_random_range(NULL, state.order, state.size);
	thread_start(&state);
	win_create(&state.win, loop, state.img.width, state.img.height);
	setup_events(&state);
	win_start(&state.win);
}

/* TODO: destroy scene and other things that might be leaking */
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
	return (EXIT_FAILURE);
}
