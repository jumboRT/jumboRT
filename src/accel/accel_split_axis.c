#include "accel.h"
#include "accel_impl.h"

/*
ik heb deze functie erg geoptimized, dit is het idee:

je krijgt in de gesorteerde lijst vaak meerdere edges met dezelfde offset
direct na elkaar, EDGE_START komt dan altjid voor EDGE_END, bijvoorbeeld:
INDEX |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9
OFFSET| 0.1 | 0.1 | 0.2 | 0.2 | 0.3 | 0.3 | 0.3 | 0.4 | 0.4 | 0.4
TYPE  |START|START|START| END |START|START| END | END | END | END
               ^                       ^           ^     ^

sommige van deze edges kunnen sowieso niet de beste zijn om op te splitten
- elke START edge met dezelfde offset als de START edge direct ervoor
    is slecht omdat de edge ervoor *altijd* beter is
- elke END edge met dezelfde offset als de END edge direct erna
    is slecht omdat de edge erna *altijd* beter is
dus in een reeks van START en END edges met dezelfde offset kan alleen de
eerste START edge of de laatste END edge de beste zijn.
in het voorbeeld hierboven aangegeven welke edges er slecht zijn met een ^
deze edges kunnen we overheen springen en callen dan nooit get_split_cost

mijn implementatie werkt als volgt:

- verzamel alle opeenvolgende edges met dezelfde offset in `edge_count`
- als er >=1 START edges waren, probeer een split met oude `prim_count`
    dit is hetzelfde als de eerste START edge van die offset
- update `prim_count` afhankelijk van de `edge_count`
- als er >=1 END edges waren, probeer een split met nieuwe `prim_count`
    dit is hetzelfde als de laatste END edge van die offset
- herhaal alles tot er geen edges meer over zijn
*/
static void
	world_best_split_axis_iter(uint32_t prim_edge_count[2][2],
			const t_edge *edge_end[2], t_split *current)
{
	current->offset = edge_end[0]->offset;
	prim_edge_count[1][EDGE_START] = 0;
	prim_edge_count[1][EDGE_END] = 0;
	while (1)
	{
		prim_edge_count[1][edge_end[0]->type] += 1;
		edge_end[0] += 1;
		if (edge_end[0] == edge_end[1]
			|| edge_end[0]->offset != current->offset)
			break ;
	}
}

static void
	world_best_split_axis_init(t_node_info *node, uint8_t axis,
			const t_edge *edge_end[2], uint32_t prim_edge_count[2][2])
{
	edge_end[0] = node->edges->edges[axis];
	edge_end[1] = edge_end[0] + node->edges->count;
	prim_edge_count[0][ACCEL_BELOW] = 0;
	prim_edge_count[0][ACCEL_ABOVE] = node->edges->count / 2;
}

static void
	world_best_split_axis_update(t_node_info *node, t_split *current,
		uint32_t prim_edge_count[2][2], t_split *best)
{
	current->cost = get_split_cost(node->bounds, current,
			prim_edge_count[0]);
	if (current->cost < best->cost)
				*best = *current;
}

void
	world_best_split_axis(t_node_info *node, t_split *best, uint8_t axis)
{
	const t_edge	*edge_end[2];
	t_split			current;
	uint32_t		prim_edge_count[2][2];
	int				in_bounds;

	current.axis = axis;
	world_best_split_axis_init(node, axis, edge_end, prim_edge_count);
	while (edge_end[0] != edge_end[1])
	{
		world_best_split_axis_iter(prim_edge_count, edge_end, &current);
		in_bounds = (current.offset > xyz(node->bounds.min, axis)
				&& current.offset < xyz(node->bounds.max, axis));
		if (in_bounds && prim_edge_count[1][EDGE_START] > 0)
			world_best_split_axis_update(node, &current, prim_edge_count, best);
		prim_edge_count[0][ACCEL_BELOW] += prim_edge_count[1][EDGE_START];
		prim_edge_count[0][ACCEL_ABOVE] -= prim_edge_count[1][EDGE_END];
		if (in_bounds && prim_edge_count[1][EDGE_END] > 0)
			world_best_split_axis_update(node, &current, prim_edge_count, best);
	}
}
