#include "work.h"

t_result
	work_compute(t_world *world, t_context *ctx, size_t index)
{
	t_result	result;

	(void) ctx;
	result.color = world->color;
	result.index = index;
	return (result);
}
