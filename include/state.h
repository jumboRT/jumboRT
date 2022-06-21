#ifndef STATE_H
# define STATE_H

# include "rtmath.h"
# include "world.h"
# include "cl.h"

# include <stddef.h>

typedef struct s_pixel	t_pixel;
typedef struct s_image	t_image;
typedef struct s_state	t_state;

struct s_pixel {
	t_vec	color;
	size_t	samples;
};

struct s_image {
	t_pixel	*data;
	size_t	width;
	size_t	height;
};

struct s_state {
	t_image	*image;
	t_world	*world;
};

#endif
