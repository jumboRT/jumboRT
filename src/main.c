#include "work.h"
#include "util.h"
#include "world_impl.h"
#include "parser.h"

#include <ft_printf.h>
#include <stdlib.h>
#include <unistd.h>

void
	world_gen(t_world *world)
{
	t_shape_sphere	sphere;
	size_t			i;

	sphere.base.shape_type = RT_SHAPE_SPHERE;
	sphere.base.mat_index = 0;
	sphere.radius = 0.04;
	i = 0;
	while (i < 400)
	{
		sphere.pos = vec(1.0, i % 20 / 10.0 - 1, (size_t) (i / 20) / 10.0 - 1);
		world_add_primitive(world, &sphere, sizeof(sphere));
		i += 1;
	}
	init_camera(world);
}

void
	world_load(t_world *world, const char *filename)
{
	char		*file;
	t_parse_ctx	ctx;
	char		*error;

	file = rt_readfile(filename, &error, NULL);
	if (file == NULL)
	{
		ft_fprintf(STDERR_FILENO, "Error\n%s", error);
		rt_free(error);
		exit(EXIT_FAILURE);
	}
	ctx.data = file;
	ctx.filename = filename;
	ctx.line = 1;
	ctx.column = 1;
	rt_world(world, &ctx);
	rt_free(file);
}

int
	main(void)
{
	t_image			image;
	t_world			world;
	t_state			state;
	t_work			work;
	size_t			i;

	image.width = 1920;
	image.height = 1080;
	image.data = rt_malloc(sizeof(*image.data) * image.width * image.height);
	i = 0;
	while (i < image.width * image.height)
	{
		image.data[i].samples = 0;
		image.data[i].color = vec(0, 0, 0);
		i += 1;
	}
	state.image = &image;
	state.world = &world;
	world_create(&world);
	world.img_meta.width = image.width;
	world.img_meta.height = image.height;
	world.img_meta.samples = 100;
	world_load(&world, "scenes/monkey.rt");
	work_create(&work, &state);
	work.work_size = world.img_meta.width * world.img_meta.height * world.img_meta.samples;
	work.work_index = 0;
	work.work_progress = 0;
	work_resume(&work);
	while (work.work_progress < work.work_size)
	{
		work_update(&work);
		usleep(1000);
	}
	rt_write_ppm("image.ppm", &image);
	work_destroy(&work);
	return (EXIT_SUCCESS);
}
