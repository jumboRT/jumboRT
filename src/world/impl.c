#include "world_impl.h"

#include "util.h"

#include <libft.h>

void
	*world_zero(uint32_t *count, uint64_t *size, uint64_t *capacity)
{
	if (count != NULL)
		*count = 0;
	*size = 0;
	*capacity = 0;
	return (NULL);
}

void
	world_create(t_world *world)
{
	world->primitives = world_zero(&world->primitives_count, &world->primitives_size, &world->primitives_capacity);
	world->materials = world_zero(&world->materials_count, &world->materials_size, &world->materials_capacity);
	world->vertices = world_zero(&world->vertices_count, &world->vertices_size, &world->vertices_capacity);
	world->accel_nodes = world_zero(&world->accel_nodes_count, &world->accel_nodes_size, &world->accel_nodes_capacity);
	world->accel_indices = world_zero(&world->accel_indices_count, &world->accel_indices_size, &world->accel_indices_capacity);
	world->accel_degenerates = world_zero(&world->accel_degenerates_count, &world->accel_degenerates_size, &world->accel_degenerates_capacity);
	world->textures = world_zero(&world->textures_count, &world->textures_size, &world->textures_capacity);
	world->texture_data = world_zero(NULL, &world->texture_data_size, &world->texture_data_capacity);
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

uint32_t
	world_add_accel_degenerate(t_world *world, uint32_t accel_degenerate)
{
	size_t	old_size;

	world->accel_degenerates_count += 1;
	old_size = world->accel_degenerates_size;
	world->accel_degenerates_size += sizeof(accel_degenerate);
	world->accel_degenerates = world_reallog(world->accel_degenerates, &world->accel_degenerates_capacity, world->accel_degenerates_size);
	ft_memcpy((char *) world->accel_degenerates + old_size, &accel_degenerate, sizeof(accel_degenerate));
	return (old_size / sizeof(accel_degenerate));
}

uint32_t
	world_add_tex(t_world *world, const t_tex *tex)
{
	size_t	old_size;

	world->textures_count += 1;
	old_size = world->textures_size;
	world->textures_size += sizeof(*tex);
	world->textures = world_reallog(world->textures, &world->textures_capacity, world->textures_size);
	ft_memcpy((char *) world->textures + old_size, tex, sizeof(*tex));
	return (old_size / sizeof(*tex));
}

uint32_t
	world_alloc_tex_data(t_world *world, size_t size)
{
	size_t	old_size;

	old_size = world->texture_data_size;
	world->texture_data_size += sizeof(*world->texture_data) * size;
	world->texture_data = world_reallog(world->texture_data, &world->texture_data_capacity, world->texture_data_size);
	return (old_size);
}

int64_t
	world_get_material(const t_world *world, uint32_t id)
{
	size_t				offset;
	const t_material	*material;

	offset = 0;
	while (offset < world->materials_size)
	{
		material = (const t_material *) ((const char *) world->materials + offset);
		if (material->id == id)
			return (offset / RT_MATERIAL_ALIGN);
		offset += sizeof(*material); /* TOOD do something like world_material_size */
	}
	return (-1);
}

