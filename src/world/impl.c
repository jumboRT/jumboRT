#include "world_impl.h"

#include "util.h"

#include <libft.h>

void
	world_create(t_world *world)
{
	world->primitives = NULL;
	world->materials = NULL;
	world->vertices = NULL;
	world->accel_nodes = NULL;
	world->accel_indices = NULL;
	world->primitives_count = 0;
	world->materials_count = 0;
	world->vertices_count = 0;
	world->accel_nodes_count = 0;
	world->accel_indices_count = 0;
	world->primitives_size = 0;
	world->materials_size = 0;
	world->vertices_size = 0;
	world->accel_nodes_size = 0;
	world->accel_indices_size = 0;
	world->primitives_capacity = 0;
	world->materials_capacity = 0;
	world->vertices_capacity = 0;
	world->accel_nodes_capacity = 0;
	world->accel_indices_capacity = 0;
}

void
	world_destroy(t_world *world)
{
	rt_free(world->primitives);
	rt_free(world->materials);
	rt_free(world->vertices);
	rt_free(world->accel_nodes);
	rt_free(world->accel_indices);
}

void
	*world_reallog(void *ptr, uint64_t *capacity, uint64_t size)
{
	size_t	capacity_size;

	capacity_size = *capacity;
	ptr = rt_reallog(ptr, &capacity_size, size);
	*capacity = capacity_size;
	return (ptr);
}

uint32_t
	world_add_material(t_world *world, void *material, size_t size)
{
	size_t	old_size;

	world->materials_count += 1;
	old_size = world->materials_size;
	world->materials_size += (size + RT_MATERIAL_ALIGN - 1) / RT_MATERIAL_ALIGN * RT_MATERIAL_ALIGN;
	world->materials = world_reallog(world->materials, &world->materials_capacity, world->materials_size);
	ft_memcpy((char *) world->materials + old_size, material, size);
	return (old_size / RT_MATERIAL_ALIGN);
}

uint32_t
	world_add_primitive(t_world *world, void *primitive, size_t size)
{
	size_t	old_size;

	world->primitives_count += 1;
	old_size = world->primitives_size;
	world->primitives_size += (size + RT_PRIMITIVE_ALIGN - 1) / RT_PRIMITIVE_ALIGN * RT_PRIMITIVE_ALIGN;
	world->primitives = world_reallog(world->primitives, &world->primitives_capacity, world->primitives_size);
	ft_memcpy((char *) world->primitives + old_size, primitive, size);
	return (old_size / RT_PRIMITIVE_ALIGN);
}

uint32_t
	world_add_vertex(t_world *world, t_vertex *vertex)
{
	size_t	old_size;

	world->vertices_count += 1;
	old_size = world->vertices_size;
	world->vertices_size += sizeof(*vertex);
	world->vertices = world_reallog(world->vertices, &world->vertices_capacity, world->vertices_size);
	ft_memcpy((char *) world->vertices + old_size, vertex, sizeof(*vertex));
	return (old_size / sizeof(*vertex));
}

uint32_t
	world_add_accel_node(t_world *world, t_accel_node *accel_node)
{
	size_t	old_size;

	world->accel_nodes_count += 1;
	old_size = world->accel_nodes_size;
	world->accel_nodes_size += sizeof(*accel_node);
	world->accel_nodes = world_reallog(world->accel_nodes, &world->accel_nodes_capacity, world->accel_nodes_size);
	ft_memcpy((char *) world->accel_nodes + old_size, accel_node, sizeof(*accel_node));
	return (old_size / sizeof(*accel_node));
}

uint32_t
	world_add_accel_index(t_world *world, uint32_t accel_index)
{
	size_t	old_size;

	world->accel_indices_count += 1;
	old_size = world->accel_indices_size;
	world->accel_indices_size += sizeof(accel_index);
	world->accel_indices = world_reallog(world->accel_indices, &world->accel_indices_capacity, world->accel_indices_size);
	ft_memcpy((char *) world->accel_indices + old_size, &accel_index, sizeof(accel_index));
	return (old_size / sizeof(accel_index));
}

