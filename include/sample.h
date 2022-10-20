#ifndef SAMPLE_H
# define SAMPLE_H

# include "world.h"

t_vec	triangle_sample(t_triangle triangle, GLOBAL t_context *ctx);
t_vec	sphere_sample(t_sphere sphere, GLOBAL t_context *ctx);
t_vec	cylinder_sample(t_cylinder cylinder, GLOBAL t_context *ctx);
t_vec	cone_sample(t_cone cone, GLOBAL t_context *ctx);

float	triangle_area(t_triangle triangle);
float	sphere_area(t_sphere sphere);
float	cylinder_area(t_cylinder cylinder);
float	cone_area(t_cone cone);

#endif
