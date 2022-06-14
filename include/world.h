#ifndef WORLD_H
# define WORLD_H

# include "rtmath.h"
# include "cl.h"

typedef struct s_image_meta	t_image_meta;
typedef struct s_world		t_world;

struct s_image_meta {
	uint64_t	width;
	uint64_t	height;
	uint64_t	samples;
};

struct s_world {
	t_image_meta	img_meta;
	t_vec			color;
};

#endif
