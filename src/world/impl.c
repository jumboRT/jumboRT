/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   impl.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:29 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:29 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include "util.h"

static void
	*world_zero(t_uint32 *count, t_uint64 *size, t_uint64 *capacity)
{
	if (count != NULL)
		*count = 0;
	*size = 0;
	*capacity = 0;
	return (NULL);
}

static void
	world_create_vectors(t_world *world)
{
	world->primitives = world_zero(&world->primitives_count,
			&world->primitives_size, &world->primitives_capacity);
	world->materials = world_zero(&world->materials_count,
			&world->materials_size, &world->materials_capacity);
	world->vertices = world_zero(&world->vertices_count,
			&world->vertices_size, &world->vertices_capacity);
	world->accel_nodes = world_zero(&world->accel_nodes_count,
			&world->accel_nodes_size, &world->accel_nodes_capacity);
	world->accel_indices = world_zero(&world->accel_indices_count,
			&world->accel_indices_size, &world->accel_indices_capacity);
	world->accel_degenerates = world_zero(&world->accel_degenerates_count,
			&world->accel_degenerates_size, &world->accel_degenerates_capacity);
	world->textures = world_zero(&world->textures_count,
			&world->textures_size, &world->textures_capacity);
	world->texture_data = world_zero(NULL,
			&world->texture_data_size, &world->texture_data_capacity);
	world->bxdfs = world_zero(&world->bxdfs_count,
			&world->bxdfs_size, &world->bxdfs_capacity);
	world->lights = world_zero(&world->lights_count,
			&world->lights_size, &world->lights_capacity);
	world->leaf_data = world_zero(&world->leaf_data_count,
			&world->leaf_data_size, &world->leaf_data_capacity);
}

void
	world_create(t_world *world)
{
	world_create_vectors(world);
	world->flags = 0;
	world->render_mode = RT_RENDER_MODE_DEFAULT;
	world->batch_size = 16;
	world->trace_batch_size = 1;
	world->ambient_dist = RT_HUGE_VAL;
}

void
	world_destroy(t_world *world)
{
	rt_free(world->primitives);
	rt_free(world->materials);
	rt_free(world->vertices);
	rt_free(world->accel_nodes);
	rt_free(world->accel_indices);
	rt_free(world->accel_degenerates);
	rt_free(world->textures);
	rt_free(world->texture_data);
	rt_free(world->bxdfs);
	rt_free(world->lights);
	rt_free(world->leaf_data);
}

void
	*world_reallog(void *ptr, t_uint64 *capacity, t_uint64 size)
{
	size_t	capacity_size;

	capacity_size = *capacity;
	ptr = rt_reallog(ptr, &capacity_size, size);
	*capacity = capacity_size;
	return (ptr);
}
