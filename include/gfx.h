#ifndef GFX_H
# define GFX_H

# include "rtmath.h"

typedef struct s_win	t_win;
typedef	struct s_img	t_img;

typedef	unsigned int	t_color;

struct s_win {
	void	*win;
	void	*mlx;
};

struct s_img {
	t_color	*data;
	int		width;
	int		height;
};

int	win_create(t_win *win, int width, int height);
int	win_destroy(t_win *win);

void	win_render(t_win *win, t_img *img);

void	img_create(t_img *img, int width, int height);
void	img_destroy(t_img *img);

void	img_clear(t_img *img, t_color color);
void	img_puti(t_img *img, int x, int y, t_color color);
void	img_putf(t_img *img, int x, int y, t_vec color);

#endif