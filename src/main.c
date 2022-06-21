#include "work.h"
#include "util.h"

#include <ft_printf.h>
#include <stdlib.h>
#include <unistd.h>

int
	main(void)
{
	t_image	image;
	t_world	world;
	t_state	state;
	t_work	work;
	size_t	i;

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
	world.color = vec(1.0, 0.0, 0.0);
	world.img_meta.width = image.width;
	world.img_meta.height = image.height;
	world.img_meta.samples = 50;
	work_create(&work, &state);
	work.work_size = world.img_meta.width * world.img_meta.height * world.img_meta.samples;
	work.work_index = 0;
	work.work_progress = 0;
	work_resume(&work);
	while (work.work_progress < work.work_size)
	{
		work_update(&work);
		usleep(100000);
	}
	rt_write_ppm("image.ppm", &image);
	work_destroy(&work);
	return (EXIT_SUCCESS);
}
