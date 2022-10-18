#include "accel.h"
#include "accel_impl.h"

#include "ser.h"
#include "perf.h"
#include "world.h"
#include "util.h"

static void
	world_accel_load_unpack(t_world *world, unsigned char *data_ptr[2])
{
	data_ptr[1] = rt_upacku64(data_ptr[1], &world->accel_nodes_size);
	data_ptr[1] = rt_upacku64(data_ptr[1], &world->accel_indices_size);
	data_ptr[1] = rt_upacku64(data_ptr[1], &world->accel_degenerates_size);
	data_ptr[1] = rt_upacku64(data_ptr[1], &world->leaf_data_size);
}

static void
	world_accel_load_alloc(t_world *world)
{
	world->accel_nodes = rt_realloc(world->accel_nodes,
			world->accel_nodes_capacity, world->accel_nodes_size);
	world->accel_indices = rt_realloc(world->accel_indices,
			world->accel_indices_capacity, world->accel_indices_size);
	world->accel_degenerates = rt_realloc(world->accel_degenerates,
			world->accel_degenerates_capacity, world->accel_degenerates_size);
	world->leaf_data = rt_realloc(world->leaf_data, world->leaf_data_capacity,
			world->leaf_data_size);
	world->accel_nodes_capacity = world->accel_nodes_size;
	world->accel_indices_capacity = world->accel_indices_size;
	world->accel_degenerates_capacity = world->accel_degenerates_size;
	world->leaf_data_capacity = world->leaf_data_size;
	world->accel_nodes_count
		= world->accel_nodes_size / sizeof(*world->accel_nodes);
	world->accel_indices_count
		= world->accel_indices_size / sizeof(*world->accel_indices);
	world->accel_degenerates_count
		= world->accel_degenerates_size / sizeof(*world->accel_degenerates);
	world->leaf_data_count = world->leaf_data_size / sizeof(*world->leaf_data);
}

static void
	world_accel_load_cpy(t_world *world, unsigned char *data_ptr[2])
{
	rt_memcpy(world->accel_nodes, data_ptr[1], world->accel_nodes_size);
	data_ptr[1] += world->accel_nodes_size;
	rt_memcpy(world->accel_indices, data_ptr[1], world->accel_indices_size);
	data_ptr[1] += world->accel_indices_size;
	rt_memcpy(world->accel_degenerates, data_ptr[1],
		world->accel_degenerates_size);
	data_ptr[1] += world->accel_degenerates_size;
	rt_memcpy(world->leaf_data, data_ptr[1], world->leaf_data_size);
	data_ptr[1] += world->leaf_data_size;
}

int
	world_accel_load(t_world *world, const char *file, uint64_t hash)
{
	unsigned char	*data_ptr[2];
	char			*error;
	size_t			size;
	uint64_t		file_hash;

	error = NULL;
	data_ptr[0] = (unsigned char *) rt_readfile(file, &error, &size);
	if (data_ptr[0] == NULL)
		return (rt_free(error), 0);
	if (size < sizeof(uint64_t) * 5)
		return (rt_free(error), 0);
	data_ptr[1] = data_ptr[0];
	data_ptr[1] = rt_upacku64(data_ptr[1], &file_hash);
	if (hash != file_hash)
		return (rt_free(error), 0);
	world_accel_load_unpack(world, data_ptr);
	world_accel_load_alloc(world);
	world_accel_load_cpy(world, data_ptr);
	rt_free(data_ptr[0]);
	return (1);
}
