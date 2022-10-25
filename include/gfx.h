/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   gfx.h                                          #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:43:49 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:43:49 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef GFX_H
# define GFX_H

# include "rtmath.h"
# include "image.h"

# ifdef RT_LINUX
#  include <X11/Xlib.h>
/*
#  define RT_WIN_EVENT_KEY_DOWN KeyPress
#  define RT_WIN_EVENT_KEY_MASK KeyPressMask
#  define RT_WIN_EVENT_BUTTON_DOWN ButtonPress
#  define RT_WIN_EVENT_BUTTON_MASK ButtonPressMask
*/
#  define RT_WIN_EVENT_KEY_DOWN 2
#  define RT_WIN_EVENT_KEY_MASK 1
#  define RT_WIN_EVENT_BUTTON_DOWN 4
#  define RT_WIN_EVENT_BUTTON_MASK 4

#  define RT_WIN_EVENT_CLOSE 17
# else
#  define RT_WIN_EVENT_KEY_DOWN 2
#  define RT_WIN_EVENT_KEY_MASK 0
#  define RT_WIN_EVENT_BUTTON_DOWN 0
#  define RT_WIN_EVENT_BUTTON_MASK 0
#  define RT_WIN_EVENT_CLOSE 17
# endif

typedef struct s_win	t_win;

struct s_win {
	void	*handle;
	void	*mlx;
	void	*img;
	char	*data;
	int		bpp;
};

void	*mlx(void);
int		win_create(t_win *win, int width, int height);

void	win_key_hook(t_win *win,
			int (*callback)(int key, void *ctx), void *ctx);
void	win_event_hook(t_win *win, int event, int (*callback)(), void *ctx);
void	win_start(int (*loop_proc)(void *ctx), void *ctx);
void	win_put(t_win *win, const t_image *img);

#endif
