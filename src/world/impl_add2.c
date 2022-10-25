/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   impl_add2.c                                    #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:27 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:27 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "world_impl.h"
#include "accel.h"
#include "util.h"

t_uint32
	world_add_accel_node(t_world *world, t_accel_node *accel_node)
{
	size_t	old_size;

	world->accel_nodes_count += 1;
	old_size = world->accel_nodes_size;
	world->accel_nodes_size += sizeof(*accel_node);
	world->accel_nodes = world_reallog(world->accel_nodes,
			&world->accel_nodes_capacity, world->accel_nodes_size);
	rt_memcpy((char *) world->accel_nodes + old_size,
		accel_node, sizeof(*accel_node));
	return (old_size / sizeof(*accel_node));
}

t_uint32
	world_add_accel_index(t_world *world, t_uint32 accel_index)
{
	size_t	old_size;

	world->accel_indices_count += 1;
	old_size = world->accel_indices_size;
	world->accel_indices_size += sizeof(accel_index);
	world->accel_indices = world_reallog(world->accel_indices,
			&world->accel_indices_capacity, world->accel_indices_size);
	rt_memcpy((char *) world->accel_indices + old_size,
		&accel_index, sizeof(accel_index));
	return (old_size / sizeof(accel_index));
}

t_uint32
	world_add_accel_degenerate(t_world *world, t_uint32 accel_degenerate)
{
	size_t	old_size;

	world->accel_degenerates_count += 1;
	old_size = world->accel_degenerates_size;
	world->accel_degenerates_size += sizeof(accel_degenerate);
	world->accel_degenerates = world_reallog(world->accel_degenerates,
			&world->accel_degenerates_capacity, world->accel_degenerates_size);
	rt_memcpy((char *) world->accel_degenerates + old_size,
		&accel_degenerate, sizeof(accel_degenerate));
	return (old_size / sizeof(accel_degenerate));
}

t_uint32
	world_add_leaf_data(t_world *world, t_leaf_data leaf_data)
{
	size_t	old_size;

	world->leaf_data_count += 1;
	old_size = world->leaf_data_size;
	world->leaf_data_size += sizeof(leaf_data);
	world->leaf_data = world_reallog(world->leaf_data,
			&world->leaf_data_capacity, world->leaf_data_size);
	rt_memcpy((char *) world->leaf_data + old_size,
		&leaf_data, sizeof(leaf_data));
	return (old_size / sizeof(leaf_data));
}
