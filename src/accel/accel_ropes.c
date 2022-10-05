#include "accel.h"
#include "accel_impl.h"

#include "util.h"

#if ACCEL_USE_ROPES

static void
	optimize(t_world *world, t_rope_data *rope_data, uint32_t index)
{
	t_accel_node	*node;

	node = world->accel_nodes + rope_data->ropes[index];
	while (!is_leaf(*node))
	{
		if (split_axis(*node) == index)
			node = world->accel_nodes + above_child(*node);
		else if (split_axis(*node) == index % 3)
			node = node + 1;
		else if (split_pos(*node) <= rope_data->bounds[split_axis(*node) + 0])
			node = world->accel_nodes + above_child(*node);
		else if (split_pos(*node) >= rope_data->bounds[split_axis(*node) + 3])
			node = node + 1;
		else
			break ;
	}
	rope_data->ropes[index] = node - world->accel_nodes;
}

static void
	process_node(t_world *world, t_accel_node *node, t_rope_data rope_data)
{
	t_rope_data	child_rope_data;
	t_leaf_data	leaf_data;
	uint32_t	i;

	i = 0;
	while (i < 6)
	{
		if (rope_data.ropes[i] != 0xFFFFFFFF)
			optimize(world, &rope_data, i);
		i += 1;
	}
	if (is_leaf(*node))
	{
		leaf_data.rope_data = rope_data;
		node->leaf_data_index = world_add_leaf_data(world, leaf_data);
	}
	else
	{
		child_rope_data = rope_data;
		child_rope_data.bounds[split_axis(*node) + 0] = split_pos(*node);
		child_rope_data.ropes[split_axis(*node) + 0] = (node - world->accel_nodes) + 1;
		process_node(world, world->accel_nodes + above_child(*node), child_rope_data);
		child_rope_data = rope_data;
		child_rope_data.bounds[split_axis(*node) + 3] = split_pos(*node);
		child_rope_data.ropes[split_axis(*node) + 3] = above_child(*node);
		process_node(world, node + 1, child_rope_data);
	}
}

void
	world_attach_ropes(t_world *world, t_bounds bounds)
{
	t_rope_data	rope_data;

	rope_data.bounds[0] = x(bounds.min);
	rope_data.bounds[1] = y(bounds.min);
	rope_data.bounds[2] = z(bounds.min);
	rope_data.bounds[3] = x(bounds.max);
	rope_data.bounds[4] = y(bounds.max);
	rope_data.bounds[5] = z(bounds.max);
	rope_data.ropes[0] = 0xFFFFFFFF;
	rope_data.ropes[1] = 0xFFFFFFFF;
	rope_data.ropes[2] = 0xFFFFFFFF;
	rope_data.ropes[3] = 0xFFFFFFFF;
	rope_data.ropes[4] = 0xFFFFFFFF;
	rope_data.ropes[5] = 0xFFFFFFFF;
	process_node(world, world->accel_nodes, rope_data);
}

#else

void
	world_attach_ropes(t_world *world, t_bounds bounds)
{
	(void) world;
	(void) bounds;
}

#endif
