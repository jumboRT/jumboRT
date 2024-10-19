/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main_viewer.c                                  #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:20 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/27 13:06:47 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parser.h"
#include "perf.h"
#include "util.h"
#include "world_impl.h"

#include <libft.h>

void
	world_load(t_world *world, const char *filename, const char *key)
{
	t_parse_ctx	ctx;

	rt_assert(ft_strlen(filename) > 2, "filename does not end with .rt");
	rt_assert(ft_strcmp(filename + ft_strlen(filename) - 3, ".rt") == 0,
		"filename does not end with .rt");
	parser_init(&ctx, filename, key);
	rt_world(world, &ctx);
	parser_destroy(&ctx);
}

static void
	main_world_init(t_options *options, t_image *image,
			t_state *state, t_world *world)
{
	image->width = options->width;
	image->height = options->height;
	image->data = rt_malloc(sizeof(*image->data)
			* image->width * image->height);
	state->image = image;
	state->world = world;
	world_create(world);
	world->img_meta.width = image->width;
	world->img_meta.height = image->height;
	world->img_meta.start_x = options->start_x;
	world->img_meta.start_y = options->start_y;
	world->img_meta.end_x = options->end_x_set ? options->end_x : image->width;
	world->img_meta.end_y = options->end_y_set ? options->end_y : image->height;
	world->batch_size = options->batch_size;
	world->trace_batch_size = options->trace_batch_size;
	world->render_mode = options->default_rendering_mode;
}

void
	main_viewer(t_options *options)
{
	t_image		image;
	t_world		world;
	t_state		state;
	t_work		work;
	t_perf		perf;

	main_world_init(options, &image, &state, &world);
	perf_start(&perf);
	world_load(&world, options->scene_file, options->key);
	perf_split(&perf, "load world");
	world_accel(&world);
	perf_split(&perf, "build tree");
	work_create(&work, &state, options, NULL);
	perf_split(&perf, "init device");
	work.work_size = (world.img_meta.end_x - world.img_meta.start_x)
		* (world.img_meta.end_y - world.img_meta.start_y) * options->samples;
	work_reset(&work);
	main_run(options, &work);
	world_destroy(&world);
	work_destroy(&work);
}
