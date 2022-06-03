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
	win_event_hook(t_win *win, int event, int (*proc)(), void *ctx)
{
	if (event == RT_WIN_EVENT_KEY_DOWN)
		mlx_hook(win->handle, event, RT_WIN_EVENT_KEY_MASK, proc, ctx);
	else if (event == RT_WIN_EVENT_BUTTON_DOWN)
		mlx_hook(win->handle, event, RT_WIN_EVENT_BUTTON_MASK, proc, ctx);
	else
		mlx_hook(win->handle, event, 0, proc, ctx);
}
