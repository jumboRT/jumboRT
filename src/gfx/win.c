#include "gfx.h"

#include "util.h"
#include <mlx.h>

int
	win_create(t_win *win, t_loop_proc proc, int width, int height)
{
	int	nothing;

	win->mlx = mlx();
	win->handle = mlx_new_window(win->mlx, width, height, "new window");
	win->img = mlx_new_image(win->mlx, width, height);
	win->data = mlx_get_data_addr(win->img, &win->bpp, &nothing, &nothing);
	mlx_loop_hook(win->mlx, proc, win);
	return (win->handle == NULL);
}

int
	win_destroy(t_win *win)
{
	mlx_clear_window(win->mlx, win->handle);
	mlx_destroy_window(win->mlx, win->handle);
	mlx_destroy_image(win->mlx, win->img);
	return (0);
}

void
	win_start(t_win *win)
{
	mlx_loop(win->mlx);
}

void
	*mlx(void)
{
	static void	*mlx = NULL;

	if (mlx == NULL)
	{
		mlx = mlx_init();
		rt_assert(mlx != NULL, "failed to initialize mlx");
	}
	return (mlx);
}
