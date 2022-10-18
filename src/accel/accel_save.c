#include "accel.h"

#include "util.h"
#include "ser.h"
#include "world.h"

void
	world_accel_save(t_world *world, const char *file, uint64_t hash)
{
	size_t			size;
	unsigned char	*data;
	unsigned char	*ptr;
	char			*error;

	size = sizeof(uint64_t) * 5;
	size += world->accel_nodes_size + world->accel_indices_size
		+ world->accel_degenerates_size + world->leaf_data_size;
	data = rt_malloc(size);
	ptr = data;
	ptr = rt_packu64(ptr, hash);
	ptr = rt_packu64(ptr, world->accel_nodes_size);
	ptr = rt_packu64(ptr, world->accel_indices_size);
	ptr = rt_packu64(ptr, world->accel_degenerates_size);
	ptr = rt_packu64(ptr, world->leaf_data_size);
	rt_memcpy(ptr, world->accel_nodes, world->accel_nodes_size);
	ptr += world->accel_nodes_size;
	rt_memcpy(ptr, world->accel_indices, world->accel_indices_size);
	ptr += world->accel_indices_size;
	rt_memcpy(ptr, world->accel_degenerates, world->accel_degenerates_size);
	ptr += world->accel_degenerates_size;
	rt_memcpy(ptr, world->leaf_data, world->leaf_data_size);
	ptr += world->leaf_data_size;
	rt_writefile(file, &error, data, size);
	rt_free(data);
}
