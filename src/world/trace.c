#include "world.h"
#include "hit.h"
#include "shape.h"
#include "mat.h"
#include "bsdf.h"

static void
	world_trace_init(const GLOBAL t_world *world,
			GLOBAL t_context *ctx, t_trace_ctx *tctx, uint64_t index)
{
	const GLOBAL t_material	*ambient;
	int						max_volumes;

	max_volumes = RT_MAX_VOLUMES;
	tctx->world = world;
	tctx->ctx = ctx;
	tctx->head = vec(1, 1, 1, 0);
	tctx->tail = vec(0, 0, 0, 0);
	tctx->volume_size = 0;
	tctx->time = world->ambient_dist;
	tctx->specref = 1;
	tctx->alpha_skips = 0;
	tctx->ray = project(world, ctx, index);
	if (world->flags & RT_WORLD_HAS_AMBIENT)
	{
		ambient = get_mat_const(world, world->ambient_mat);
		if ((ambient->flags & RT_MAT_HAS_VOLUME) && max_volumes > 0)
		{
			tctx->volumes[tctx->volume_size] = ambient;
			tctx->volume_size += 1;
		}
	}
}

t_vec
	world_trace(const GLOBAL t_world *world, GLOBAL t_context *ctx,
			uint64_t begin, int depth)
{
	t_trace_ctx	tctx;
	t_vec		result;
	uint64_t	i;
	int			this_depth;

	result = vec_0();
	i = 0;
	while (i < world->trace_batch_size)
	{
		this_depth = depth;
		world_trace_init(world, ctx, &tctx, begin + i);
		while (this_depth > 0 && world_trace_step(&tctx))
			this_depth -= 1;
		result = vec_add(result, tctx.tail);
		i += 1;
	}
	return (vec_scale(result, 1.0 / world->trace_batch_size));
}

#ifdef RT_OPENCL

void
	world_trace_all(const GLOBAL t_world *world, GLOBAL t_context *ctx,
			const t_trace_all_ctx *t)
{
	int				depth;
	t_trace_ctx		tctx;
	uint64_t		i[2];

	depth = 0;
	i[0] = world->trace_batch_size;
	i[1] = atomic_add(t->index, 1);
	while (i[1] < t->end - t->begin)
	{
		if (depth == 0)
		{
			if (i[0] == world->trace_batch_size)
				t->results[i[1]].color = vec_0();
			i[0] %= world->trace_batch_size;
			depth = RT_MAX_DEPTH;
			world_trace_init(world, ctx, &tctx, t->begin + i[1]);
		}
		depth = (depth - 1) * !!world_trace_step(&tctx);
		if (depth == 0)
			t->results[i[1]].color = vec_scale(vec_add(t->results[i[1]].color,
						tctx.tail), 1.0f / world->trace_batch_size);
		if (depth == 0 && ++i[0] == world->trace_batch_size)
			i[1] = atomic_add(t->index, 1);
	}
}

#endif
