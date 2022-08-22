#include "work.h"
#include "util.h"
#include "world.h"
#include "world_impl.h"
#include "parser.h"
#include "perf.h"
#include "net.h"


#include <ft_printf.h>
#include <stdlib.h>
#include <unistd.h>
#include "z.h"


#include <stdio.h>

#define FLY_SPEED 1

__attribute__((noreturn))
int
	rt_exit(t_work *work)
{
	work_pause(work);
	work_update_stop(work);
	work_destroy(work);
	exit(EXIT_SUCCESS);
}

void
	rt_work_lock(t_work *work)
{
	work_pause(work);
	mutex_lock(&work->state_mtx);
	while (work->work_progress < work->work_index)
		cond_wait(&work->progress_cnd, &work->state_mtx);
	work_reset(work);
	mutex_unlock(&work->state_mtx);
}

void
	rt_work_unlock(t_work *work)
{
	work_resume(work);
}

sig_atomic_t should_exit(sig_atomic_t incr)
{
	static sig_atomic_t	val = 0;

	val += incr;
	return val;
}

#if defined RT_BONUS && !defined RT_JOINC

static void
	sigint_handler(int sig)
{
	(void) sig;
	should_exit(1);
}

#endif

static void
	setup_sighandlers(void)
{
#if defined RT_BONUS && !defined RT_JOINC
	struct sigaction	action;

	rt_assert(sigemptyset(&action.sa_mask) != -1, "failed to empty sa_mask");
	action.sa_handler = sigint_handler;
	action.sa_flags = SA_NODEFER | SA_RESTART;
	rt_assert(sigaction(SIGINT, &action, NULL) != -1, "failed to install signal handler");
#else
	printf("warning: you're using the mandatory configuration or running on windows. NEVER forcefully kill this process!\n");
#endif
}

static void
	main_image(t_work *work, const char *image_file)
{
	t_perf	perf;

	setup_sighandlers();
	work_update_start(work);
	perf_start(&perf);
	work_resume(work);
	while (1)
	{
		mutex_lock(&work->state_mtx);
		if (work->work_progress >= work->work_size || should_exit(0))
		{
			mutex_unlock(&work->state_mtx);
			break ;
		}
		mutex_unlock(&work->state_mtx);
		usleep(10000);
	}
	perf_split(&perf, "draw image");
	mutex_lock(&work->state_mtx);
	rt_write_ppm(image_file, work->state->image);
	mutex_unlock(&work->state_mtx);
	perf_split(&perf, "save image");
	rt_exit(work);
}

#ifndef RT_JOINC

#include "keycode.h"

int
	rt_key_down(int keycode, void *ctx)
{
	t_work *work;
	t_camera *camera;
	t_vec dir;
	t_vec org;
	t_vec left;

	work = ctx;
	if (keycode == RT_KEY_ESC || keycode == RT_KEY_Q)
		rt_exit(work);
	camera = &work->state->world->camera;
	org = camera->org;
	dir = work->state->world->camera.dir;
	left = vec_norm(vec_cross(dir, vec_z(1.0)));
	if (keycode == RT_KEY_R)
	{
		rt_work_lock(work);
		printf("%f %f %f %f\n", x(org), y(org), z(org), camera->fov);
		rt_work_unlock(work);
	}
	if (keycode == RT_KEY_LEFT)
	{
		dir = vec_rotate(vec_z(1.0), dir, -RT_PI / (12 * (90.0 / camera->fov)));
		rt_work_lock(work);
		camera_set(work->state->world, camera, org, dir, camera->fov);
		rt_work_unlock(work);
	}
	if (keycode == RT_KEY_RIGHT)
	{
		dir = vec_rotate(vec_z(1.0), dir, RT_PI / (12 * (90.0 / camera->fov)));
		rt_work_lock(work);
		camera_set(work->state->world, camera, org, dir, camera->fov);
		rt_work_unlock(work);
	}
	if (keycode == RT_KEY_UP)
	{
		dir = vec_rotate(left, dir, RT_PI / (12 * (90 / camera->fov)));
		rt_work_lock(work);
		camera_set(work->state->world, camera, org, dir, camera->fov);
		rt_work_unlock(work);
	}
	if (keycode == RT_KEY_DOWN)
	{
		dir = vec_rotate(left, dir, -RT_PI / (12 * (90 / camera->fov)));
		rt_work_lock(work);
		camera_set(work->state->world, camera, org, dir, camera->fov);
		rt_work_unlock(work);
	}
	if (keycode == RT_KEY_S)
	{
		org = vec_add(org, vec_scale(vec_neg(dir), FLY_SPEED));	    
		rt_work_lock(work);
		camera_set(work->state->world, camera, org, dir, camera->fov);
		rt_work_unlock(work);
	}
	if (keycode == RT_KEY_W)
	{
		org = vec_add(org, vec_scale(dir, FLY_SPEED));	    
		rt_work_lock(work);
		camera_set(work->state->world, camera, org, dir, camera->fov);
		rt_work_unlock(work);
	}
	if (keycode == RT_KEY_A)
	{
		org = vec_add(org, vec_scale(left, FLY_SPEED));
		rt_work_lock(work);
		camera_set(work->state->world, camera, org, dir, camera->fov);
		rt_work_unlock(work);
	}
	if (keycode == RT_KEY_D)
	{
	    org = vec_add(org, vec_scale(vec_neg(left), FLY_SPEED));
	    rt_work_lock(work);
	    camera_set(work->state->world, camera, org, dir, camera->fov);
	    rt_work_unlock(work);
	}
	if (keycode == RT_KEY_SPACE)
	{
	    org = vec_add(org, vec_z(FLY_SPEED));
	    rt_work_lock(work);
	    camera_set(work->state->world, camera, org, dir, camera->fov);
	    rt_work_unlock(work);
	}
	if (keycode == RT_KEY_SHIFT)
	{
	    org = vec_add(org, vec_neg(vec_z(FLY_SPEED)));
	    rt_work_lock(work);
	    camera_set(work->state->world, camera, org, dir, camera->fov);
	    rt_work_unlock(work);
	}
	if (keycode == RT_KEY_1)
	{
		rt_work_lock(work);
		work->state->world->render_mode = RT_RENDER_MODE_DEFAULT;
	    rt_work_unlock(work);
	}
	if (keycode == RT_KEY_2)
	{
		rt_work_lock(work);
		work->state->world->render_mode = RT_RENDER_MODE_UV;
	    rt_work_unlock(work);
	}
	if (keycode == RT_KEY_3)
	{
		rt_work_lock(work);
		work->state->world->render_mode = RT_RENDER_MODE_GEOMETRIC_NORMAL;
	    rt_work_unlock(work);
	}
	if (keycode == RT_KEY_4)
	{
		rt_work_lock(work);
		work->state->world->render_mode = RT_RENDER_MODE_SHADING_NORMAL;
	    rt_work_unlock(work);
	}
	if (keycode == RT_KEY_Z)
	{
		rt_work_lock(work);
		camera_set(work->state->world, camera, org, dir, rt_max(5.0, camera->fov - 5));
		rt_work_unlock(work);
	}
	if (keycode == RT_KEY_X) {
		rt_work_lock(work);
		camera_set(work->state->world, camera, org, dir, rt_min(179.0, camera->fov + 5));
		rt_work_unlock(work);
	}
	return (0);
}

int
	rt_loop(void *ctx)
{
	t_work	*work;

	work = ctx;
	if (should_exit(0))
		rt_exit(work);
	mutex_lock(&work->state_mtx);
	win_put(&work->state->win, work->state->image);
	mutex_unlock(&work->state_mtx);
	return (0);
}

static void
	main_window(t_work *work)
{
	setup_sighandlers();
	work_update_start(work);
	work_resume(work);
	win_create(&work->state->win, work->state->image->width, work->state->image->height);
	win_event_hook(&work->state->win, RT_WIN_EVENT_KEY_DOWN, rt_key_down, work);
	win_event_hook(&work->state->win, RT_WIN_EVENT_CLOSE, rt_exit, work);
	win_start(rt_loop, work);
}

static void
	main_run(t_options *options, t_work *work)
{
	if (options->image_file == NULL)
		main_window(work);
	else
		main_image(work, options->image_file);
}

#else

static void
	main_run(t_options *options, t_work *work)
{
	if (options->image_file == NULL)
	{
		ft_fprintf(STDERR_FILENO, "Cannot open window in joinc mode\n");
		exit(EXIT_FAILURE);
	}
	else
		main_image(work, options->image_file);
}

#endif

/*
#define RT_VERBOSE
void
	dump_tree(t_world *world, uint32_t offset, int depth, t_vec min, t_vec max)
{
	int				i;
	t_accel_node	*node;
	t_vec			vec;

	i = 0;
	while (i < depth)
	{
		fprintf(stdout, "  ");
		i += 1;
	}
	node = &world->accel_nodes[offset];
	if (is_leaf(*node))
	{
#ifdef RT_VERBOSE
		if (nprims(*node) == 0)
			fprintf(stdout, "leaf %d (%f %f %f | %f %f %f), none (offset %d)\n", (int) nprims(*node), x(min), y(min), z(min), x(max), y(max), z(max), offset);
		else if (nprims(*node) == 1)
			fprintf(stdout, "leaf %d (%f %f %f | %f %f %f), one %d (offset %d)\n", (int) nprims(*node), x(min), y(min), z(min), x(max), y(max), z(max), node->a.one_primitive, offset);
		else
			fprintf(stdout, "leaf %d (%f %f %f | %f %f %f), first %d (offset %d)\n", (int) nprims(*node), x(min), y(min), z(min), x(max), y(max), z(max), world->accel_indices[node->a.primitive_ioffset], offset);
#else
		fprintf(stdout, "leaf %d\n", (int) nprims(*node));
#endif
	}
	else
	{
#ifdef RT_VERBOSE
		fprintf(stdout, "branch (%f %f %f | %f %f %f) (%f %d) (offset %d)\n", x(min), y(min), z(min), x(max), y(max), z(max), split_pos(*node), split_axis(*node), offset);
#else
		fprintf(stdout, "branch\n");
#endif
		vec = vec_set(max, split_axis(*node), split_pos(*node));
		dump_tree(world, offset + 1, depth + 1, min, vec);
		vec = vec_set(min, split_axis(*node), split_pos(*node));
		dump_tree(world, above_child(*node), depth + 1, vec, max);
	}
}
*/

void
	world_load(t_world *world, const char *filename, const char *key)
{
	t_parse_ctx	ctx;

	parser_init(&ctx, filename, key);
	rt_world(world, &ctx);
	parser_destroy(&ctx);
}

void
	main_viewer(t_options *options)
{
	t_image		image;
	t_world		world;
	t_state		state;
	t_work		work;
	t_perf		perf;

	image.width = options->width;
	image.height = options->height;
	image.data = rt_malloc(sizeof(*image.data) * image.width * image.height);
	state.image = &image;
	state.world = &world;
	world_create(&world);
	world.img_meta.width = image.width;
	world.img_meta.height = image.height;
	perf_start(&perf);
	world_load(&world, options->scene_file, options->key);
	perf_split(&perf, "load world");
	world_accel(&world);
	perf_split(&perf, "build tree");
	work_create(&work, &state, options, NULL);
	perf_split(&perf, "init device");
	work.work_size = world.img_meta.width * world.img_meta.height * options->samples;
	work_reset(&work);
	main_run(options, &work);
	/* TODO: become thanos */
	work_destroy(&work);
}

int
	main(int argc, char **argv)
{
	t_options		options;
	union u_client	client;
	

	void	*tmp;
	size_t	size;

	tmp = lz77_deflate("ababcbababaa", 12, &size);
	fprintf(stderr, "size:%zu\n", size);
	write(1, tmp, size * sizeof(t_ztoken));
	rt_free(tmp);
	return (0);

	parse_options(&options, argc, argv);
	if (options.worker)
	{
		rt_worker_create(&client, options, options.net_ip, options.net_port);
		rt_client_start(&client);
	}
	else
	{
		main_viewer(&options);
	}
	return (EXIT_SUCCESS);
}
