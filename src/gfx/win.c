#include "gfx.h"

#include "state.h"
#include <mlx.h>
#include <stddef.h>

void
	*mlx(void)
{
	static void	*mlx = NULL;

	if (mlx == NULL)
		mlx = mlx_init();
	return (mlx);
}

int	win_create(t_win *win, int width, int height)
{
	int	ignore;

	win->handle = mlx_new_window(mlx(), width, height, "JUMBO");
	win->img = mlx_new_image(mlx(), width, height);
	win->data = mlx_get_data_addr(win->img, &win->bpp, &ignore, &ignore);
	return (0);
}

void
	win_start(int (*loop_proc)(void *ctx), void *ctx)
{
	mlx_loop_hook(mlx(), loop_proc, ctx);
	mlx_loop(mlx());
}

void
	win_key_hook(t_win *win, int (*callback)(int key, void *ctx), void *ctx)
{
	mlx_key_hook(win->handle, callback, ctx);
}

void
	win_event_hook(t_win *win, int event, int (*callback)(), void *ctx)
{
	if (event == RT_WIN_EVENT_KEY_DOWN)
		mlx_hook(win->handle, event, RT_WIN_EVENT_KEY_MASK, callback, ctx);
	else if (event == RT_WIN_EVENT_BUTTON_DOWN)
		mlx_hook(win->handle, event, RT_WIN_EVENT_BUTTON_MASK, callback, ctx);
	else
		mlx_hook(win->handle, event, 0, callback, ctx);
}
