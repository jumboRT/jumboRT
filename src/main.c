#include <stdio.h>
#include <stdlib.h>
#include "rtmath.h"
#include "gfx.h"
#include <mlx.h>

int
	loop(void *handle)
{
	t_img	img;
	t_win	*win;

	win = handle;
	img_create(&img, 600, 400);
	img_clear(&img, COLOR_GREEN);
	win_put(win, &img);
	img_destroy(&img);
	(void) handle;
	return (0);
}

int
	main(void)
{
	t_win	win;

	win_create(&win, loop, 600, 400);
	win_start(&win);
	return (EXIT_SUCCESS);
}
