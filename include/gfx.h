#ifndef GFX_H
# define GFX_H

# include "rtmath.h"
# include "util.h"
# include "scene.h"
# include "mt.h"

typedef struct s_win	t_win;
typedef struct s_rt_win	t_rt_win;
typedef struct s_img	t_img;
typedef int				(*t_loop_proc)(void *ctx);

# define COLOR_RED		0x00ff0000
# define COLOR_BLUE		0x000000ff
# define COLOR_GREEN	0x0000ff00

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

void	*mlx(void);
int		win_create(t_win *win, t_loop_proc proc, int width, int height);
int		win_destroy(t_win *win);
void	win_start(t_win *win);
void	win_put(t_win *win, t_img *img);

void	win_render(t_win *win, t_img *img);

void	img_create(t_img *img, int width, int height);
void	img_destroy(t_img *img);

void	img_clear(t_img *img, t_color color);
void	img_puti(t_img *img, int x, int y, t_color color);
void	img_putf(t_img *img, int x, int y, t_vec color);

#endif
