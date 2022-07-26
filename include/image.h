#ifndef IMAGE_H
# define IMAGE_H

#include "rtmath.h"

#include <stddef.h>

typedef struct s_pixel	t_pixel;
typedef struct s_image	t_image;

struct s_pixel {
    t_vec   color;
    size_t  samples;
};

struct s_image {
    t_pixel *data;
    size_t  width;
    size_t  height;
};

#endif
