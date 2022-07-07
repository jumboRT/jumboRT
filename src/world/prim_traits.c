#include "world.h"

int prim_is_infinite(const t_primitive *prim) {
	return (prim->shape_type == RT_SHAPE_PLANE);
}
