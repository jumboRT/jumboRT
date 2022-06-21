#include "world_impl.h"

#include "util.h"

#include <libft.h>

void
	world_create(t_world *world)
{
	world->primitives = NULL;
	world->materials = NULL;
	world->vertices = NULL;
	world->primitives_size = 0;
	world->materials_size = 0;
	world->vertices_size = 0;
}

void
	world_destroy(t_world *world)
{
	rt_free(world->primitives);
	rt_free(world->materials);
	rt_free(world->vertices);
}

uint32_t
	world_add_material(t_world *world, void *material, size_t size)
{
	size_t	old_size;

	old_size = world->materials_size;
	world->materials_size += (size + RT_MATERIAL_ALIGN - 1) / RT_MATERIAL_ALIGN * RT_MATERIAL_ALIGN;
	world->materials = rt_realloc(world->materials, old_size, world->materials_size);
	ft_memcpy((char *) world->materials + old_size, material, size);
	return (old_size / RT_MATERIAL_ALIGN);
}

uint32_t
	world_add_primitive(t_world *world, void *primitive, size_t size)
{
	size_t	old_size;

	old_size = world->primitives_size;
	world->primitives_size += (size + RT_PRIMITIVE_ALIGN - 1) / RT_PRIMITIVE_ALIGN * RT_PRIMITIVE_ALIGN;
	world->primitives = rt_realloc(world->primitives, old_size, world->primitives_size);
	ft_memcpy((char *) world->primitives + old_size, primitive, size);
	return (old_size / RT_PRIMITIVE_ALIGN);
}

uint32_t
	world_add_vertex(t_world *world, t_vertex *vertex)
{
	size_t	old_size;

	old_size = world->vertices_size;
	world->vertices_size += sizeof(*vertex);
	world->vertices = rt_realloc(world->vertices, old_size, world->vertices_size);
	ft_memcpy((char *) world->vertices + old_size, vertex, sizeof(*vertex));
	return (old_size / sizeof(*vertex));
}

