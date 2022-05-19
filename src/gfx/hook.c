#include "gfx.h"

#include <mlx.h>

void
	win_mouse_hook(t_win *win, int (*proc)(void*), void *ctx)
{
	mlx_mouse_hook(win->handle, proc, ctx);
}

void
	win_key_hook(t_win *win, int (*proc)(void*), void *ctx)
{
	mlx_key_hook(win->handle, proc, ctx);
}

void
	win_event_hook(t_win *win, int event, int (*proc)(void*), void *ctx)
{
	mlx_hook(win->handle, event, 0, proc, ctx);
}
