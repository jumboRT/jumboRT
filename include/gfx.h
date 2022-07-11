#ifndef GFX_H
# define GFX_H

# include "mt.h"
# include "rtmath.h"

struct s_img {
	t_color	*data;
	int		width;
	int		height;
	t_mutex	mtx;
};

struct s_win {
	void	*handle;
	void	*mlx;
	void	*img;
	char	*data;
	int		bpp;
};

int	win_create(t_win *win, int (*loop_proc)(void*ctx), int width, int height);
void win_start(t_win *win);
void win_put(t_win *win, t_img *img);

#endif
