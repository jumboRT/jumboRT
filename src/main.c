#include "work.h"
#include "util.h"

#include <ft_printf.h>
#include <stdlib.h>

int
	main(void)
{
	t_image	image;
	t_world	world;
	t_state	state;
	t_work	work;

	image.width = 1920;
	image.height = 1080;
	image.data = rt_malloc(sizeof(*image.data) * image.width * image.height);
	state.image = &image;
	state.world = &world;
	work_create(&work, &state);
	return (EXIT_SUCCESS);
}
