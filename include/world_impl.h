#ifndef WORLD_IMPL_H
# define WORLD_IMPL_H

# include "world.h"
# include "state.h"

# include <stddef.h>

void		world_create(t_world *world);
void		world_destroy(t_world *world);

uint32_t	world_add_material(t_world *world, void *material, size_t size);
uint32_t	world_add_primitive(t_world *world, void *shape, size_t size);
uint32_t	world_add_vertex(t_world *world, t_vertex *vertex);

#endif
